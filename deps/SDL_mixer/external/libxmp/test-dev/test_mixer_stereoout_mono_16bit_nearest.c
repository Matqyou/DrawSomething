#include "test.h"

TEST(test_mixer_stereoout_mono_16bit_nearest)
{
	compare_mixer_samples(
		"data/mixer_stereoout_mono_16bit_nearest.data", "data/test.xm",
		8000, 0, XMP_INTERP_NEAREST, TEST_XM_SAMPLE_16BIT_MONO, 0);
}
END_TEST
