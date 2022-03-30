static const int MAXNROFLIGHTS = 6;//if change, change this in Graphics 2
cbuffer lightOCameraCBuf : register (b6)
{
	float4 cameraPos;
	float4 lightColor[MAXNROFLIGHTS];
    float4 lightPos[MAXNROFLIGHTS]; //apperently rotation shits on the position
    row_major matrix lightViewProj[MAXNROFLIGHTS];
	int nrOfLight; 
}
