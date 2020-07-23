#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float time;
const int STEPS = 20;
//uniform float t; // time component
//uniform int STEPS; // The number of adjacent locations in one direction to use for smearing
// function that does not perform any transform
vec2 f0(float x, float y) {
    return vec2(0.0, 0.0);
}

vec2 f1(float x, float y) {
    return vec2(0.5*sin(5.0*x*x*cos(time) + 5.0*y), 0.4*cos(5.0*x*x*sin(time) + 5.0*y));
}

vec2 f2(float x, float y) {
    return vec2(0.4*sin(x + 10.0*sin(time)*y) + y, 0.4*sin(x + 10.0*sin(time)*y) - x*sqrt(abs(x)));
}

vec2 f3(float x, float y) {
    return vec2(sin(10.0*x*sin(time) + y), cos(2.0*x*sin(time) + 11.0*y*cos(time)));
}

vec2 f4(float x, float y) {
    return vec2(0.5*sin(5.0*x*x*cos(time) + 5.0*y*sqrt(abs(y))) + y, 0.4*cos(5.0*x*x*sin(time) + 5.0*y*sqrt(abs(y))) - x);
}

vec2 f5(float x, float y) {
    return vec2(sin(2.0*y + 3.0*x*sin(time)), cos(cos(time)*5.0*sqrt(abs(x))));
}

vec2 f6(float x, float y) {
    return vec2(y*cos(time), -x*sin(time));
}

// function to use for the x and y portions of the vector field
vec2 f(float x, float y) {
    int t_t = (int(time)/3);
    int t_s = t_t - 6*(t_t/6);
    int s = t_s + 1;
    if(s == 1) {
        return f1(x, y);
    } else if(s == 2) {
        return f2(x, y);
    } else if(s == 3) {
        return f3(x, y);
    } else if(s == 4) {
        return f4(x, y);
    } else if(s == 5) {
        return f5(x, y);
    }

    return f6(x, y);
}

// perfrom line integral convolution
vec4 lic() {
    vec2 coords = v_texcoord0;
    const float STEPSIZE = 0.001; // Stepsize to access adjacent texture sample

    vec2 init_coords = coords; // inital x,y coordinate, shift domain from (0,1) domain to (-1,1)
    vec2 v; // the vector field's x and y components
    vec2 step_coords; // current step x,y coordinate
    vec4 texture_value = vec4(0.0); // color value at the particular texture coordinate vec4/RGBA
    float running_total = 0.0; // running total of grey-scale texture values
//
//    // Start at this fragment's location
    step_coords = init_coords;

    // step FORWARD along the vector field
    for(int i=0; i<STEPS; i++) {
        v = f(step_coords[0], step_coords[1]);

        // use Euler's Method. Get the next approximate point along the curve
        step_coords += STEPSIZE * v;

        // get the texture value at that point
        texture_value = texture2D(aTexture0, step_coords); // color value at the particular texture coordinate vec4/RGBA
        running_total += texture_value[0]; // It is a grayscale texture, so just use R in RGBA
    }
//
//    // resart at this fragment's location
    step_coords = init_coords;
//
//    // step BACKWARD along the vector field
    for(int i=0; i<STEPS; i++) {
        v = f(step_coords[0], step_coords[1]);

        // use Euler's Method. Get the next approximate point along the curve
        step_coords -= STEPSIZE * v;

        // get the texture value at that point
        texture_value = texture2D(aTexture0, step_coords); // color value at the particular texture coordinate vec4/RGBA
        running_total += texture_value[0]; // It is a grayscale texture, so just use R in RGBA
    }
//
    // average grayscale value of all samples along the curve, this is the final value for this pixel
    float average_value = running_total / float(STEPS * 2);
    // use average value for RGB and 1 for A
    vec4 return_value = vec4(average_value,average_value,average_value,1.0);

    return return_value;
    
//    return vec4(1.0, 0.0, 1.0, 1.0);
}

void main() {
    gl_FragColor = lic();   // run lic; note the return value is of type vec4/RGBA
}


