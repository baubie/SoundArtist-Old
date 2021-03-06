//
//  SoundFile.m
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-11.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import "SoundFile.h"


@implementation SoundFile

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (BOOL)openFile:(NSString *)filename {
    
    SF_INFO info;
	SNDFILE* sf = sf_open([filename UTF8String], SFM_READ, &info);
    
    if (sf == NULL) {
        NSRunAlertPanel(@"File Error", 
                        @"Error loading sound file. Are you sure this is a valid sound file?", 
                        nil, nil, nil);
		return false;
	}
	if (info.channels != 1) {
        NSRunAlertPanel(@"File Error", 
                        @"Error loading sound file. This file has more than one channel. Only single channel sound files are supported.", 
                        nil, nil, nil);
		return false;
	}
    
    m_info = info;
	m_filename = filename;
	sf_close(sf);
    
	return true;
}

- (int) sampleRate {
    return m_info.samplerate;
}

- (int) frames {
    return (int)m_info.frames;
}

- (double) duration {
    return 1000*(double)m_info.frames / m_info.samplerate;
}

- (void) runGLE: (NSString *) filename
{
    NSTask *gleProcess;
    gleProcess = [[NSTask alloc] init];    
    [gleProcess setLaunchPath:@"/Applications/QGLE.app/Contents/bin/gle"];
    [gleProcess setArguments:
    [NSArray arrayWithObjects:@"-d",@"pdf", filename, nil]];
    [gleProcess launch];
    [gleProcess waitUntilExit];
    [gleProcess release];
}



- (void) makeWindow: (enum windows) type length:(int) N array: (float *) window {
#define PI 3.14159265
#define alpha 0.16
#define alpha0 (1-alpha)/2
#define alpha1 0.5
#define alpha2 alpha/2
    
    switch (type)
    {
            
        case Flattop:
            for(int i=0; i < N; ++i) {
                window[i] = 0.355768 - 0.487396*cos(2*PI*i/(N-1)) + 0.144232*cos(4*PI*i/(N-1)) - 0.012604*cos(6*PI*i/(N-1));
            }
            break;
            
        case Cosine:
            for(int i=0; i < N; ++i) {
                window[i] = sin(PI*i/(N-1)); 
            }            
            break;
        
        case Hann:
            for(int i=0; i < N; ++i) {
                window[i] = 0.5 * (1 - cos((2*PI*i)/(N-1)));
            }
            break;
            
        case Hamming:
            for(int i=0; i < N; ++i) {
                window[i] = 0.54 - 0.46*cos(2*PI*i/(N-1)); 
            }
            break;
            
        case Blackman:
            for(int i=0; i < N; ++i) {
                window[i] = alpha0 - alpha1*cos(2*PI*i/(N-1))+alpha2*cos(4*PI*i/(N-1));
            }
            break;
        
        default:
            for(int i=0; i < N; ++i) {
                window[i] = 1.0;
            }
            break;

        
    }
}

- (BOOL) makeSpectrogramData: (NSString*) dataFilename info:(struct WR_SPECINFO*) specinfo {

    SNDFILE* sf = sf_open([m_filename UTF8String], SFM_READ, &m_info);    
	// Make sure our settings are valid
    if (specinfo->floor < 0) specinfo->floor = 0;	
    if (specinfo->floor > 1) specinfo->floor = 1;
    
    
	// Read in the sound file
	double *buf;
	buf = (double *) malloc((int)m_info.frames*sizeof(double));
    sf_read_double(sf,buf,(int)m_info.frames);    
	sf_close(sf);
    
    
	int timescale = 1;
	if (specinfo->timeaxis == milliseconds) timescale = 1000;
    
	float freqscale = 0.001;
	if (specinfo->freqaxis == hz) freqscale = 1;
    
	int N = specinfo->nwindow;
    
    
	// Get the window
	float window[N];    
    [self makeWindow:specinfo->window length:N array: window];   
    
    // Create data file header
	int nx = floor((m_info.frames-N)/specinfo->increment)+1;
	int ny = N/2.0+1;
    
    NSMutableString* data = [[NSMutableString alloc] init];    
    
    float xmin, xmax;
	if (specinfo->startatzero) {
        xmin = (float)-1.0*timescale*specinfo->start/1000;
        xmax = (float)timescale*((float)m_info.frames/(float)m_info.samplerate) - specinfo->start*timescale/1000;
	} else {
        xmin = 0;
        xmax = (float)timescale*m_info.frames/m_info.samplerate;        
	}
    
	[data appendFormat:@"! nx %i ny %i xmin %f xmax %f ymin 0.0 ymax %f\n", nx, ny, xmin, xmax, freqscale*m_info.samplerate/2.0];
    
	// Write spectrogram data to file
	
	// Create out 2D array on the heap (due to size)
    
    double **spec;
    spec = malloc(nx * sizeof(double *));
    for (int i = 0; i < nx; ++i) {
        spec[i] = malloc(ny * sizeof(double));
    }
    
    // Initialize array
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
            spec[i][j] = 0;
    
	int curX = 0;
	double maxVal = -99999999;
    double minVal = +99999999;
	double *in;
	double *out;
	in = (double*) fftw_malloc(sizeof(double)*specinfo->nwindow);
	out = (double*) fftw_malloc(sizeof(double)*specinfo->nwindow);
	fftw_plan p = fftw_plan_r2r_1d(specinfo->nwindow, in, out, FFTW_R2HC, FFTW_ESTIMATE);
    
    if (specinfo->increment < 1) specinfo->increment = 1;
    
	for (int i = 0; i < m_info.frames-N; i+=specinfo->increment)
	{
		for (int j = 0; j < N; ++j)
		{              
			in[j] = buf[i+j]*window[j];	
		}
        
		fftw_execute(p);
        
		// Save results
		for (int j = 1; j < ny; ++j)
		{
			double val = 10*log10((out[j]*out[j]+out[N-j]*out[N-j]));
            if (out[j]*out[j]+out[N-j]*out[N-j] == 0) {
                val = -99999999;
            }
			spec[curX][j] = val;
			maxVal = maxVal < spec[curX][j] ? spec[curX][j] : maxVal;
            minVal = minVal > spec[curX][j] ? spec[curX][j] : minVal;
		}
		curX++;
	}
	fftw_destroy_plan(p);
	fftw_free(in); 
	fftw_free(out);
    
    
	// Write the data to file
	double floor = 1-specinfo->floor;
	for (int y = 0; y < ny; ++y) {
		for (int x = 0; x < nx; ++x) {
            
            if (spec[x][y] > -99999999)
            {
                double val = 1-(spec[x][y]-minVal)/(maxVal-minVal);
                
                // This is our floor function.  Right now we shoot everything to 1.
                // Should have something more gradual.
                if (val > floor) val = 1;
                else val = 1-pow((val-floor)/(floor),2);
                
                [data appendFormat:@"%f ", val];
            } else {
                [data appendString:@"- "];
            }
		}
        [data appendString:@"\n"];
	}
	free(buf);
    for (int i = 0; i < nx; ++i) {
        free(spec[i]);
    }
    free(spec);
    
    NSError *error;
    [data writeToFile:dataFilename atomically:NO encoding:NSUTF8StringEncoding error:&error];
    [data release]; 
    
    return true;
}

- (NSString*) makeSpectrogramPlot: (NSString*) gleFilename info:(struct WR_SPECINFO*) specinfo {

    int timescale = 1;
	if (specinfo->timeaxis == milliseconds) timescale = 1000;
    
	float freqscale = 0.001;
	if (specinfo->freqaxis == hz) freqscale = 1;
    
	int N = specinfo->nwindow;
	int nx = floor((m_info.frames-N)/specinfo->increment)+1;
	int ny = N/2.0+1;
    
    NSMutableString* gle = [[NSMutableString alloc] init];    

    [gle appendString:@"begin graph\n"];
    [gle appendString:@"vscale auto\n"];
    [gle appendString:@"nobox\n"];
    [gle appendString:@"x2axis off\n"];
    [gle appendString:@"y2axis off\n"];
    
    if (specinfo->timeaxis == milliseconds) {
        [gle appendString:@"xtitle \"Time (ms)\"\n"];
	}
	else {
        [gle appendString:@"xtitle \"Time (s)\"\n"];
	}
	if (specinfo->freqaxis == hz) {
        [gle appendString:@"ytitle \"Frequency (Hz)\"\n"];
	} else {
        [gle appendString:@"ytitle \"Frequency (kHz)\"\n"];
	}
    
    [gle appendString:@"xticks length -0.1\n"];
    [gle appendString:@"yticks length -0.1\n"];
    [gle appendString:@"title \"\"\n"];
	if (specinfo->startatzero) {
        [gle appendFormat:@"xaxis min %f max %f\n", 0.0, (specinfo->end-specinfo->start)*timescale/1000];
	} else {
        [gle appendFormat:@"xaxis min %f max %f\n", specinfo->start*timescale/1000, specinfo->end*timescale/1000];
	}
    
    [gle appendFormat:@"yaxis min %f max %f\n", 0.0, freqscale*m_info.samplerate/2];
    
    
	NSString* scalebar_palette;
	switch (specinfo->palette)
	{
		case 1:
			scalebar_palette = [NSString stringWithString:@"color"];
            [gle appendFormat:@"colormap \"%@\" %i %i color\n", @"spectrogram.z", nx, ny];
			break;
            
		default:
			scalebar_palette = [NSString stringWithString:@"grayscale"];
            [gle appendFormat:@"colormap \"%@\" %i %i\n", @"spectrogram.z", nx, ny];
			break;
	}
    
    [gle appendString:@"end graph\n"];   

    return gle;
}


- (BOOL) makeSpectrogram:(NSString *)folder info:(struct WR_SPECINFO *) specinfo {   
    
    NSString *zFilename = [[NSString alloc] initWithFormat:@"%@/spectrogram.z", folder];
    NSString *gleFilename = [[NSString alloc] initWithFormat:@"%@/spectrogram.gle", folder];


    [self makeSpectrogramData:zFilename info:specinfo];
    

    
	// Create the GLE file.
    NSMutableString* gle = [[NSMutableString alloc] init];    
    [gle appendFormat:@"size %f %f\n", specinfo->width, specinfo->height];
    [gle appendString:@"include \"color.gle\"\n"];
    [gle appendString:@"set font psh\n"];
    [gle appendFormat:@"set hei %f\n", specinfo->fontsize];
    [gle appendFormat:@"set lwidth %f\n", specinfo->lwidth];
    [gle appendString:[self makeSpectrogramPlot:zFilename info:specinfo]];
    
    
    /*
	if (specinfo->scalebar)
	{
        [gle appendString:@"amove xg(xgmax)+0.5 yg(ygmin)\n"];
		[gle appendFormat:@"color_range_vertical zmin 0 zmax 1 zstep 50 palette \"%@\" pixels 1500 format \"fix 0\"\n", scalebar_palette];
	}
     */
    
    NSError *error;
    [gle writeToFile:gleFilename atomically:NO encoding:NSUTF8StringEncoding error:&error];
    [gle release];
    
    [self runGLE: gleFilename];
    [gleFilename release];
    [zFilename release];
	return true;

}

- (BOOL) makeWaveformData: (NSString*) dataFilename info:(struct WR_WFINFO*) wfinfo {
    // Read in the sound file
	SNDFILE* sf = sf_open([m_filename UTF8String], SFM_READ, &m_info);
	double *buf;
	buf = (double *) malloc((int)m_info.frames*sizeof(double));
    sf_read_double(sf,buf,(int)m_info.frames);    
	sf_close(sf);
    
	int timescale = 1;
	if (wfinfo->timeaxis == milliseconds) timescale = 1000;
    
	// Write wavefile data to file
	
    NSMutableString* data = [[NSMutableString alloc] init];
    
    
	for (int i = 0; i < m_info.frames; i+=(int)(1.0/wfinfo->quality))
	{
		float t;
		if (wfinfo->startatzero)
		{
			t = (float)(timescale*(float)i/(float)m_info.samplerate) - wfinfo->start*timescale/1000;
		} else {
			t = (float)(timescale*(float)i/(float)m_info.samplerate);
		}
        [data appendFormat:@"%f,%f\n", t, buf[i]];
	}
	free(buf);
    NSError *error;
    [data writeToFile:dataFilename atomically:NO encoding:NSUTF8StringEncoding error:&error];
    [data release];
    
    return true;
}

- (NSString*) makeWaveformPlot: (NSString*) dataFilename info:(struct WR_WFINFO*) wfinfo
{
    int timescale = 1;
	if (wfinfo->timeaxis == milliseconds) timescale = 1000;

    NSMutableString* gle = [[NSMutableString alloc] init];
    [gle appendString:@"begin graph\n"];
    [gle appendString:@"vscale auto\n"];
    [gle appendString:@"nobox\n"];
    [gle appendString:@"x2axis off\n"];
    [gle appendString:@"y2axis off\n"];
    
	if (wfinfo->timeaxis == milliseconds) {
        [gle appendString:@"xtitle \"Time (ms)\"\n"];
	}
	else {
        [gle appendString:@"xtitle \"Time (s)\"\n"];
	}
    [gle appendString:@"ytitle \"\"\n"];
    
	if (wfinfo->ylabels == false)
	{
        [gle appendString:@"ylabels off\n"];
	}
    
    [gle appendString:@"xticks length -0.1\n"];
    [gle appendString:@"yticks length -0.1\n"];
    [gle appendString:@"title \"\"\n"];
	if (wfinfo->startatzero) {
        [gle appendFormat:@"xaxis min %f max %f\n", 0.0, (wfinfo->end-wfinfo->start)*timescale/1000];
	} else {
        [gle appendFormat:@"xaxis min %f max %f\n", wfinfo->start*timescale/1000, wfinfo->end*timescale/1000];
	}
    
    [gle appendFormat:@"data \"%@\"\n", @"waveform.dat"];
    [gle appendFormat:@"d1 line lwidth %f\n", wfinfo->wflwidth];
    [gle appendString:@"end graph\n"];
    return gle;
}


- (BOOL) makeWaveForm: (NSString*) folder info:(struct WR_WFINFO*) wfinfo {
    
    
    
    NSString *dataFilename = [[NSString alloc] initWithFormat:@"%@/waveform.dat", folder];
    NSString *gleFilename = [[NSString alloc] initWithFormat:@"%@/waveform.gle", folder];

    if (wfinfo->end < wfinfo->start) {
        wfinfo->end = wfinfo->start;
    }

    [self makeWaveformData:dataFilename info:wfinfo];
    
    
    NSMutableString* gle = [[NSMutableString alloc] init];        
    [gle appendFormat:@"size %f %f\n", wfinfo->width, wfinfo->height];
    [gle appendString:@"set font psh\n"];
    [gle appendFormat:@"set hei %f\n", wfinfo->fontsize];
    [gle appendFormat:@"set lwidth %f\n", wfinfo->lwidth];
    [gle appendString:[self makeWaveformPlot:gleFilename info: wfinfo]];

    NSError *error;    
    [gle writeToFile:gleFilename atomically:NO encoding:NSUTF8StringEncoding error:&error];
    [gle release];
    
    [self runGLE: gleFilename];
    [gleFilename release];
    [dataFilename release];
    
    return true;
}


- (BOOL) makePowerData: (NSString*) dataFilename info:(struct WR_SPECINFO*) specinfo
{
    SNDFILE* sf = sf_open([m_filename UTF8String], SFM_READ, &m_info);
    
	// Read in the sound file
	double *buf;
	buf = (double *) malloc((int)m_info.frames*sizeof(double));
    sf_read_double(sf,buf,(int)m_info.frames);    
	sf_close(sf);
    
	float freqscale = 0.001;
	if (specinfo->freqaxis == hz) freqscale = 1;
    
    int N = specinfo->nwindow;
    
	// Get the window
	float window[specinfo->nwindow];    
    [self makeWindow:specinfo->window length:specinfo->nwindow array: window];  
    
    NSMutableString* data = [[NSMutableString alloc] init];    
    
	// Write spectrogram data to file
	// Create out 2D array on the heap (due to size)            
	double *in;
	double *out;
    double *power;
    int numFreqs = specinfo->nwindow/2+1;
	in = (double*) fftw_malloc(sizeof(double)*specinfo->nwindow);
	out = (double*) fftw_malloc(sizeof(double)*specinfo->nwindow);
    power = (double*) fftw_malloc(sizeof(double)*(numFreqs));
    
	fftw_plan p = fftw_plan_r2r_1d(specinfo->nwindow, in, out, FFTW_R2HC, FFTW_ESTIMATE);
    
    for (int j = 0; j < numFreqs; ++j)
    {
        power[j] = 0;
    }
    
    int count = 0;
    for (int i = 0; i < m_info.frames-specinfo->nwindow; i+=specinfo->increment)
	{
        if (i >= specinfo->start*m_info.samplerate && i <= specinfo->end*m_info.samplerate)
        {
            for (int j = 0; j < specinfo->nwindow; ++j)
            {
                in[j] = buf[i+j]*window[j];	
            }
            
            fftw_execute(p);
            
            // Save results
            power[0] += out[0]*out[0];
            for (int j = 1; j < specinfo->nwindow/2; ++j)
            {
                double val = out[j]*out[j]+out[N-j]*out[N-j];
                power[j] += val;
            }
            if (N%2 == 0) power[N/2] += out[N/2]*out[N/2];
            count++;
        }
	}
    
    // Average over the spectrogram and find the peak
    double maxValue = -9999999;
    for (int j = 0; j < N/2 + 1; ++j)
    {
        power[j] /= count;
        maxValue = maxValue < power[j] ? power[j] : maxValue;
    }
    
    for (int j = 0; j < numFreqs; ++j)
    {
        if (power[j] > 0)
        {
            power[j] = 10*log10(power[j] / maxValue);
        } else {
            power[j] = -100000;
        }
    }
    
    // Save results        
	// Write the data to file    
    for (int k = 0; k < specinfo->nwindow/2; k+=1)
    {
        if (power[k] != - 100000)
            [data appendFormat:@"%f,%f\n",(double)k*freqscale*m_info.samplerate/specinfo->nwindow,power[k]];
        else
            [data appendFormat:@"%f,-\n",(double)k*freqscale*m_info.samplerate/specinfo->nwindow];
    }
    
    free(buf);
    free(power);
	fftw_destroy_plan(p);
	fftw_free(in); 
	fftw_free(out);

    NSError *error;
    [data writeToFile:dataFilename atomically:NO encoding:NSUTF8StringEncoding error:&error];
    [data release]; 

    return true;
}


- (NSString*) makePowerPlot: (NSString*) gleFilename info:(struct WR_SPECINFO*) specinfo
{
    float freqscale = 0.001;
	if (specinfo->freqaxis == hz) freqscale = 1;

    NSMutableString* gle = [[NSMutableString alloc] init];
    [gle appendString:@"begin graph\n"];
    [gle appendString:@"vscale auto\n"];
    [gle appendString:@"nobox\n"];
    [gle appendString:@"x2axis off\n"];
    [gle appendString:@"y2axis off\n"];
    
    if (specinfo->freqaxis == hz) {
        [gle appendString:@"xtitle \"Frequency (Hz)\"\n"];
	} else {
        [gle appendString:@"xtitle \"Frequency (kHz)\"\n"];
	}
    [gle appendString:@"ytitle \"Amplitude (dB Re Peak)\"\n"];
    [gle appendFormat:@"xaxis min 0 max %f\n", freqscale*m_info.samplerate/2];    
    [gle appendString:@"xticks length -0.1\n"];
    [gle appendString:@"yticks length -0.1\n"];
    [gle appendString:@"title \"\"\n"];
    [gle appendFormat:@"data \"%@\"\n", @"power.dat"];
    [gle appendString:@"d1 line\n"];
    [gle appendString:@"end graph\n"];   
    return gle;
}


- (BOOL) makePower:(NSString *)folder info:(struct WR_SPECINFO *) specinfo {   

    NSString *zFilename = [[NSString alloc] initWithFormat:@"%@/power.dat", folder];
    NSString *gleFilename = [[NSString alloc] initWithFormat:@"%@/power.gle", folder];
    

    [self makePowerData: zFilename info: specinfo];


    
	// Create the GLE file.
    NSMutableString* gle = [[NSMutableString alloc] init];
    [gle appendFormat:@"size %f %f\n", specinfo->width, specinfo->height];
    [gle appendString:@"set font psh\n"];
    [gle appendFormat:@"set hei %f\n", specinfo->fontsize];
    [gle appendFormat:@"set lwidth %f\n", specinfo->lwidth];
    
    [gle appendString:[self makePowerPlot:zFilename info:specinfo]];
    
    NSError *error;
    [gle writeToFile:gleFilename atomically:NO encoding:NSUTF8StringEncoding error:&error];
    [gle release];
        
    [self runGLE: gleFilename];
    
    [gleFilename release];
    [zFilename release];
    
    
	return true;
    
}


@end
