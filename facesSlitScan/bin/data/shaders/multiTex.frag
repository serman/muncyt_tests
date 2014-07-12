#version 120

uniform float timeValX = 1.0;
uniform float timeValY = 1.0;

// GPU_SlitScan 10 texturas
uniform int nFrames;
uniform sampler2DRect maskTex;

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;
uniform sampler2DRect tex4;
uniform sampler2DRect tex5;
uniform sampler2DRect tex6;
uniform sampler2DRect tex7;
uniform sampler2DRect tex8;
uniform sampler2DRect tex9;
uniform sampler2DRect tex10;
uniform sampler2DRect tex11;
uniform sampler2DRect tex12;
//uniform sampler2DRect tex13;
//uniform sampler2DRect tex14;
//uniform sampler2DRect tex15;
//uniform sampler2DRect tex16;
//uniform sampler2DRect tex17;
//uniform sampler2DRect tex18;
//uniform sampler2DRect tex19;
//uniform sampler2DRect tex20;
//uniform sampler2DRect tex21;
//uniform sampler2DRect tex22;
//uniform sampler2DRect tex23;
//uniform sampler2DRect tex24;

void main(){
	
	vec2 pos = gl_TexCoord[0].st;

	vec4 mask = texture2DRect(maskTex, pos);
	vec4[12] txts;
	txts[0] = texture2DRect(tex1, pos);
	txts[1] = texture2DRect(tex2, pos);
	txts[2] = texture2DRect(tex3, pos);
	txts[3] = texture2DRect(tex4, pos);
	txts[4] = texture2DRect(tex5, pos);
	txts[5] = texture2DRect(tex6, pos);
	txts[6] = texture2DRect(tex7, pos);
	txts[7] = texture2DRect(tex8, pos);
	txts[8] = texture2DRect(tex9, pos);
	txts[9] = texture2DRect(tex10, pos);
	txts[10] = texture2DRect(tex11, pos);
	txts[11] = texture2DRect(tex12, pos);
//	txts[12] = texture2DRect(tex13, pos);
//	txts[13] = texture2DRect(tex14, pos);
//	txts[14] = texture2DRect(tex15, pos);
//	txts[15] = texture2DRect(tex16, pos);
//	txts[16] = texture2DRect(tex17, pos);
//	txts[17] = texture2DRect(tex18, pos);
//	txts[18] = texture2DRect(tex19, pos);
//	txts[19] = texture2DRect(tex20, pos);
//	txts[20] = texture2DRect(tex21, pos);
//	txts[21] = texture2DRect(tex22, pos);
//	txts[22] = texture2DRect(tex23, pos);
//	txts[23] = texture2DRect(tex24, pos);
	
	vec4 color = vec4(1.0,0.0,1.0,1.0);

	float wRed = 1.0/nFrames;	// divido el rango de grises para el total de frames

	int nTex = int(mask.r/wRed);
	if(nTex==nFrames) nTex--;	// por si la señal mask.r==1
	color = txts[nTex];

	gl_FragColor = color;	

}