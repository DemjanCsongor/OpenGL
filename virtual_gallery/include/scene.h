#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

typedef struct Scene
{
    Model wall1;
    Model floor;
    Model img1;
    Model ufo;
    Material material;
    GLuint skybox_texture;
    GLuint floor_texture;
    GLuint wall_texture;
    GLuint img1_texture;
    GLuint img2_texture;
    GLuint img3_texture;
    GLuint img4_texture;
    GLuint img5_texture;
    GLuint img6_texture;
    GLuint ufo_texture;
    GLuint guide_texture;
    GLuint pendulum_texture;
    float brightness;
    float diffuse[3];
    float animation_path;
    bool animation_flag;
    bool animation_direction;
    bool guide_flag;
    bool fog_flag;
    //Pendulum pendulums[3];
} Scene;

/*
typedef struct {
    float length;
    float angle;
    float angular_velocity;
} Pendulum;
*/
/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(const Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Set the current material seperate back and front.
 */
void set_material2(const Material *front_material, const Material *back_material);
/**
 * Update the scene.
 */
void update_scene(Scene* scene, double time);

/**
 * Render the scene objects.
 */
void render_scene(Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

/**
 * Set the scene's brightness
 */
void setBrightness(Scene *scene, float brightness);

/**
 * Load skybox
 */
void load_skybox(Scene scene);

/**
 * Load models for init_scene function
 */
void load_models_init_scene(Scene *scene);

/**
 * Load textures for init_scene function
 */
void load_textures_init_scene(Scene *scene);

/**
 * Load objects
 */
void load_objects(Scene scene);

/**
 * Open a guide which contain the keyboard settings
 */
void show_guide(GLuint texture);

void set_fog(Scene* fog_flag);

void toggle_fog(Scene* scene);

void draw_pendulum(float length);

#endif /* SCENE_H */