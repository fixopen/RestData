//
// Created by up duan on 1/3/2017.
//

#ifndef RESTDATA_META_H
#define RESTDATA_META_H


class Meta {
};

class AudioMeta : public Meta {
};

class VideoMeta : public Meta {
};

class ImageMeta : public Meta {
};

class MP3Meta : public AudioMeta {
};

class JPEGMeta : public ImageMeta {
};

class MPEGMeta : public VideoMeta {
};

#endif //RESTDATA_META_H
