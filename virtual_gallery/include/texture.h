#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <GL/glext.h>

typedef GLubyte Pixel[3];

/**
 * Load texture from file and returns with the texture name.
 */
GLuint load_texture(char* filename);

#endif /* TEXTURE_H */
