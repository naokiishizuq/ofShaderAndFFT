//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it

uniform sampler2DRect texture1;		//second image
uniform sampler2DRect texture2;		//spectrum

#define N (256)
uniform float camWidth;
uniform float camHeight;
uniform float specArray[N];

//uniform float time;             //Uniform value "time"

//blurring
void main(){
    vec2 pos = gl_TexCoord[0].st;
    
    
    vec4 color0 = texture2DRect(texture0, pos);
    vec4 color1 =  texture2DRect(texture1, pos);	//colors converted to 0..1
    
    
    vec2 center = vec2(camWidth, camHeight) / 2.0;
    float dist = distance( center, pos );	//built-in function for distance
    
    //Aliasing
    //int index = int( dist / 7.0 );
    //index = clamp( index, 0, N-1 );
    //float spectrValue = specArray[ index ];
    
    //Antialiasing
    vec2 spectrumPos = vec2( dist / 4.0, 0.5 );
    vec4 color2 =  texture2DRect(texture2, spectrumPos);	//spectrum values
    float spectrValue = color2.r;		//get spectrum value, normally in [0..1]
    
    
    vec2 delta = pos - center;
    delta *= 1-spectrValue;
    vec2 posS = center + delta;
    
    //Audio responsible
    float manuplatedSpecValue = spectrValue * 1.15 + 0.75;
    //pat 1
//    vec4 color0 = texture2DRect(texture0, pos) * manuplatedSpecValue;
//    vec4 color1 =  texture2DRect(texture1, pos) * manuplatedSpecValue;
    
    //pat 2
//    color0 = texture2DRect(texture0, posS);
//    color1 = texture2DRect(texture1, posS);
    
    //pat 3
    color0 = texture2DRect(texture0, posS) * manuplatedSpecValue;
    color1 = texture2DRect(texture1, posS) * manuplatedSpecValue;
    
    
    vec4 color;
    
    //Case: Example
    color = min( color0, color1 );
    
    //Case: Show spectrum
//    color = vec4( 1.0-color2.rgb, 1.0 );
    
    
    
//    color = color0;		//Test image passing
    //color = color1;
    
    
    //if ( color2.r > 0.3 ) { color = color0; }
    //else { color = color1; }
    //color = mix( color0, color2, color2.r );
    
    
    
    
    //int x = int(pos.x);	//we can not convert float to int explicitly, so need use implicit conversion
    //if ( x % 2 == 0 ) {
    //	color = color1;
    //}
    
    //Output of shader
    gl_FragColor = color;
}