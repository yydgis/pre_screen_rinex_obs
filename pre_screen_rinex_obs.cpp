/* Pre Screen Rinex Obs
* by Dr. Yudan Yi
* it uses rtklib_lte from github.com/yydgis/rtklib_lte as main 3rd parties code
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <ctime>
#include <utility>

#include "rtklib.h"

/*--------------------------------------------------------------*/
/* for rtklib code purpose */
extern int showmsg(const char* format, ...)
{
    va_list arg;
    va_start(arg, format); vfprintf(stderr, format, arg); va_end(arg);
    fprintf(stderr, "\r");
    return 0;
}
extern void settspan(gtime_t ts, gtime_t te) {}
extern void settime(gtime_t time) {}
/*--------------------------------------------------------------*/

static void set_output_file_name(const char* fname, const char* key, char* outfname)
{
	char filename[255] = { 0 }, outfilename[255] = { 0 };
	strcpy(filename, fname);
	char* temp = strrchr(filename, '.');
	if (temp) temp[0] = '\0';
	sprintf(outfname, "%s-%s", filename, key);
}


static FILE* set_output_file(const char* fname, const char* key)
{
	char filename[255] = { 0 };
	set_output_file_name(fname, key, filename);
	return fopen(filename, "w");
}

/* process the data from rinex log file */
static int process_rinex_file(const char* fname, rnxctr_t* rinex, int *numofepoch, int *numof_gps_L1L2, double *dt)
{
	int i = 0, j = 0, k = 0, ret = 0, wk = 0;
	clock_t start = clock();
	FILE* fRINEX = fopen(fname, "r");

	*numofepoch = 0;
	*numof_gps_L1L2 = 0;
	int sys = 0, prn = 0;
	gtime_t stime = { 0 };
	gtime_t etime = { 0 };
	if (fRINEX && open_rnxctr(rinex, fRINEX))
	{
		while (!feof(fRINEX))
		{
			int ret = input_rnxctr(rinex, fRINEX);
			if (ret < 0) break; /* end of file */
			if (ret == 1)
			{
				int ngps = 0;
				int ngps_l1l2 = 0;
				obs_t* obs = &rinex->obs;
				for (i = 0; i < obs->n; ++i)
				{
					int prn = 0;
					int sys = satsys(obs->data[i].sat, &prn);
					if (sys == SYS_GPS)
					{
						if (fabs(obs->data[i].L[0]) > 0.001 && fabs(obs->data[i].L[1]) > 0.001 && fabs(obs->data[i].P[0]) > 0.001 && fabs(obs->data[i].P[1]) > 0.001)
						{
							++ngps_l1l2;
						}
						++ngps;
					}
				}
				if (ngps_l1l2 > 5)
				{
					if (*numof_gps_L1L2 == 0)
					{
						stime = obs->data[0].time;
					}
					etime = obs->data[0].time;
					++(*numof_gps_L1L2);
				}
				if (ngps > 0)
				{
					++(*numofepoch);
				}
			}
		}
	}

	*dt = 0;
	if (*numof_gps_L1L2 > 0)
	{
		*dt = timediff(etime, stime);
	}

	if (fRINEX) fclose(fRINEX);

	return 0;
}

static int process_rinex_obs(const char *fname)
{
	rnxctr_t* rinex = new rnxctr_t;
	init_rnxctr(rinex);

	int numofepoch = 0;
	int numofgps_l1l2 = 0;
	double dt = 0;

	process_rinex_file(fname, rinex, &numofepoch, &numofgps_l1l2, &dt);

	FILE* fOUT = set_output_file(fname, "-info.csv");

	if (fOUT)
	{
		int is_ppp_ok = 0;
		if (dt > 7200.0 && numofepoch > 0 && numofgps_l1l2 > (0.80 * numofepoch))
			is_ppp_ok = 1;
		fprintf(fOUT, "%i,%6i,%6i,%10.4f,%s\n", is_ppp_ok, numofepoch, numofgps_l1l2, dt, fname);
		fclose(fOUT);
	}

	/* house cleaning for rinex */
	free_rnxctr(rinex);
	delete rinex;
	return 0;
}


int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		process_rinex_obs(argv[1]);
	}

	return 0;
}
