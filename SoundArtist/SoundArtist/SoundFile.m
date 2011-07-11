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

@end
