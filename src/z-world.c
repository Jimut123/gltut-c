#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <math.h>

#include "z-world.h"
#include "z-maths.h"
#include "z-list.h"
#include "z-main.h"
#include "z-models.h"
#include "z-materials.h"
#include "z-pvm.h"

static float world_min_x = -1024.0f;
static float world_min_y = -1024.0f;
static float world_min_z = -1024.0f;

static float world_max_x = 1024.0f;
static float world_max_y = 1024.0f;
static float world_max_z = 1024.0f;

static int id = 0;

void prepare_world_ground(void)
{
}

void prepare_world_walls(void)
{
}

void prepare_world_sky(void)
{
}

void prepare_world_nether(void)
{
}

struct object_st *obj_create_object(char *name,
				    struct model_st *model,
				    struct material_st *mat)
{
    struct object_st *tmp;

    tmp = (struct object_st *)malloc(sizeof(struct object_st));
    if (tmp == NULL) {
	perror("Cannot allocalte memory in create_object");
	exit(EXIT_FAILURE);
    }

    vec3_zero(tmp->translate);
    vec3_set_1f(tmp->scale,1.0);
    vec3_zero(tmp->rotate_axis);
    tmp->rotate_angle = 0.0f;

    tmp->id = id++;
    tmp->name = strdup(name);
    tmp->model = model;
    tmp->material = mat;

    return tmp;
}

void destroy_object(struct object_st *obj)
{
    free(obj);
}

void world_init(float maxx, float maxy, float maxz)
{
    world_min_x = -maxx;
    world_min_y = -maxy;
    world_min_z = -maxz;

    world_max_x = maxx;
    world_max_y = maxy;
    world_max_z = maxz;

    prepare_world_walls();
    prepare_world_sky();
    prepare_world_nether();
    prepare_world_ground();

}

void obj_abs_translate(struct object_st *obj, float x, float y, float z)
{
    obj->translate[0] = x;
    obj->translate[1] = y;
    obj->translate[2] = z;
}

void obj_abs_scale(struct object_st *obj, float x, float y, float z)
{
    obj->scale[0] = x;
    obj->scale[1] = y;
    obj->scale[2] = z;
}

void obj_abs_rotate(struct object_st *obj, float x, float y, float z,
		    float a)
{
    obj->rotate_axis[0] = x;
    obj->rotate_axis[1] = y;
    obj->rotate_axis[2] = z;
    obj->rotate_angle = a;
}

void obj_set_material(struct object_st *obj, int mat)
{
    obj->material = mtrl_get_std_material(mat);
}

void obj_set_modelblk_offset(struct object_st *obj, int offset)
{
    obj->modelblk_offset = offset;
}

void obj_set_modelblk_size(struct object_st *obj, int sz)
{
    obj->modelblk_size = sz;
}

void obj_compute_model_mat4(struct object_st *obj)
{
    pvm_compute_model_mat4(obj->modelblk.modelmat,
			     obj->translate,
			     obj->scale, obj->rotate_axis, obj->rotate_angle);
}

void obj_compute_normal_mat4(struct object_st *obj)
{

    mat4_inverse(obj->modelblk.normalmat, obj->modelblk.modelmat);
    mat4_transpose(obj->modelblk.normalmat, obj->modelblk.normalmat);
}

void obj_print(struct object_st *obj)
{
    printf("Object:%s [%d]\n", obj->name, obj->id);
    printf("\tModel: %s [%d]\n", obj->model->name, obj->model->id);
    printf("\tMaterial: %s [%d]\n", obj->material->name, obj->material->id);
}
