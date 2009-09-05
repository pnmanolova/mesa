
const int KernelSize = 9;

uniform int KernelCount;
//texture offsets
uniform vec2 Offset[KernelSize];
//convolution kernel
uniform vec4 KernelValue[KernelSize];
uniform vec4 KernelScale[KernelSize];
uniform sampler2D srcTex;
uniform vec4 ScaleFactor;
uniform vec4 BaseColor;

void main(void)
{
    int i;
    vec4 sum = vec4(0.0);
    for (i = 0; i < KernelCount; ++i) {
        sum += KernelValue[i] * KernelScale[i];
    }
    gl_FragColor = sum;
}
