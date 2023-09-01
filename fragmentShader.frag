#version 330

precision highp float;

#define FloatType float

uniform FloatType zoom, w, h, precision_;
uniform FloatType complexNb_r, complexNb_i;
uniform FloatType x1, y1;
//uniform FloatType screen_y;
uniform vec2 resolution;

void main(){
    

    vec2 pos = (resolution - gl_FragCoord.xy) - vec2(w/2, resolution.y - h/2);
    //vec2 pos = gl_FragCoord.xy;
    
    FloatType z_r = ((pos.x + x1) / (zoom));
    FloatType z_i = ((pos.y + y1) / (zoom));

    FloatType i = 0.0;
    FloatType j = 0.0;

    while (z_r * z_r + z_i * z_i < 4.0 && i < precision_)
        {
        FloatType a = z_r, b = z_i;
        z_r = +1*1*a*a*1 - 1*b*b + complexNb_r;
        z_i = +2*a*1*b + complexNb_i;
        i+=1.0;
        }
    
    FloatType k = max(i,j);

    //gl_FragColor = suite(i/max_iter);
    gl_FragColor = vec4(k/(precision_),0.0,k/(precision_),1.0);
} 