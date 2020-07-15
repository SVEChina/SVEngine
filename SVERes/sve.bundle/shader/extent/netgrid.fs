#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
varying vec2 v_pos;
uniform float u_gsize;
uniform float u_border;
uniform float u_tbl_w;
uniform float u_tbl_h;
uniform vec4 u_bg_color;
uniform vec4 u_border_color;
uniform vec4 u_line_color;

void main() {
    //20像素分一道，一道4像素
    float t_half_boder = u_border*0.5;
    float t_double_boder = u_border*2.0;
    vec2 t_dert_border = mod(v_pos+vec2(u_border,u_border),vec2(u_tbl_w,u_tbl_h));
    vec2 t_dert_line = mod(v_pos+vec2(t_half_boder,t_half_boder),u_gsize);
    //
    if(t_dert_border.x<=t_double_boder) {
        gl_FragColor = u_border_color;
    }else if(t_dert_border.y<=t_double_boder) {
        gl_FragColor = u_border_color;
    }else if(t_dert_line.x<=u_border) {
        gl_FragColor = u_line_color;
    }else if(t_dert_line.y<=u_border) {
        gl_FragColor = u_line_color;
    }else{
        gl_FragColor = u_bg_color;
    }
}
