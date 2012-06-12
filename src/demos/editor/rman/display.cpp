#include <ndspy.h>
#include <iostream>

using namespace std;

PtDspyError DspyImageQuery ( PtDspyImageHandle image, PtDspyQueryType type, int size, void *data )
{
	cout << "DspyImageQuery" << endl;
	cout << "type:" << type << endl;
	cout << "size:" << size << endl;

	return PkDspyErrorNone;
}

PtDspyError DspyImageOpen ( PtDspyImageHandle * image, const char *drivername, const char *filename, int width, int height, int paramcount, const UserParameter *parameters, int formatcount, PtDspyDevFormat *format, PtFlagStuff *flags)
{
	cout << "DspyImageOpen" << endl;
	cout << "driver name: " << drivername << endl;
	cout << "filename: " << filename << endl;
	cout << "width: " << width << endl;
	cout << "height: " << height << endl;
	cout << "parameter count: " << paramcount << endl;

	for (int p = 0; p < paramcount; ++p)
	{
		cout << p << ":" << parameters[p].name << endl;
	}
	*image = (PtDspyImageHandle) new float();

	return PkDspyErrorNone;
}

PtDspyError DspyImageData ( PtDspyImageHandle image, int xmin, int xmax_plus_one, int ymin, int ymax_plus_one, int entrysize, const unsigned char *data )
{
	//cout << "DspyImageData" << endl;
	return PkDspyErrorNone;
}

PtDspyError DspyImageClose ( PtDspyImageHandle image )
{
	cout << "DspyImageClose" << endl;
	return PkDspyErrorNone;
}

//PtDspyError DspyImageDelayClose ( PtDspyImageHandle image )
//{
//	cout << "DspyImageDelayClose" << endl;
//	return PkDspyErrorNone;
//}
