//
//  SoundFile.h
//  SoundArtist
//
//  Created by Brandon Aubie on 11-07-11.
//  Copyright 2011 McMaster University. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "sndfile.h"


@interface SoundFile : NSObject {
@private
    NSString* m_filename;
    SF_INFO m_info;
}

- (BOOL) openFile: (NSString *) filename;

@end
