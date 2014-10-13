#ifndef SHOT_IMAGE_H
#define SHOT_IMAGE_H


#include <string>
#include <vector>


using std::string;
using std::vector;


namespace shot {


const int IMAGE_ERROR_OK = 0;
const int IMAGE_ERROR_READ = 1;
const int IMAGE_ERROR_SIZES = 2;
const int IMAGE_ERROR_SMALLER = 3;


// resize src image to width, height size
// rationWidth - save ratio by width, real height not equal to height
// or save ratio by height, real width not equal to width
// return error code
int resizeImage(string dest, string src, int width, int height);
int resizeGif(string dest, string src, int width, int height);
int resizeSmart(string dest, string src, int width, int height);
int resizeSmartBunch(string dest, string src, vector<int> sizes);
int resizeThumb(string dest, string src, int width, int height);


} /* namespace shot */

#endif /* end of include guard: SHOT_IMAGE_H */
