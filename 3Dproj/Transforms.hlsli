static const int MAXNROFLIGHTS = 6;
cbuffer lightOCameraCBuf : register (b6)
{
	float4 cameraPos;
	float4 lightColor;
    float4 lightPos[MAXNROFLIGHTS]; //apperently rotation shits on the position
    row_major matrix projection;
    row_major matrix lightViewProj[MAXNROFLIGHTS];
	int nrOfLight; 
}
