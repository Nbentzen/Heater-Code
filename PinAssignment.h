#pragma once
class PinAssignment
{
public:
	enum PinList
	{
		J0 = 49U,
		J1 = 48U,
		J2 = 47U,
		J3 = 6U,
		J4 = 7U,
		J5 = 4U,
		J6 = 3U,
		J7 = 42U,
		J8_2 = 62U,
		J8_4 = 63U,
		J8_6 = 64U,
		J8_8 = 65U,
		J9_2 = 66U,
		J9_4 = 67U,
		J9_6 = 68U,
		J9_8 = 69U,
		J10_1 = 55U,
		J10_2 = 56U,
		J10_3 = 56U,
		TBD_1 = 36U,
		TBD_2 = 37U,
		BUTT_1 = 2U,
    CAPILLARY_CS = 45U,
    CAPILLARY_HEAT = 44U,
    ANALYTE_CS = 43U,
    ANALYTE_HEAT = 42U,
    MASS_SPEC_CS = 41U,
    MASS_SPEC_HEAT = 35U,
    SAMPLER_CS = 39U,
    SAMPLER_HEAT = 38U,
	};

	struct ChannelData
	{
		const int index;
		PinList pin;
	};

  struct HeaterChannelData
  {
    const int index;
    PinList CsPin;
    PinList OutPin;
  };

  const struct HeaterChannelList
  {
    HeaterChannelData CapillaryHeater = {0, PinList::CAPILLARY_CS, PinList::CAPILLARY_HEAT};
    HeaterChannelData AnalyteHeater = {1, PinList::ANALYTE_CS, PinList::ANALYTE_HEAT};
    HeaterChannelData MassSpecHeater = {2, PinList::MASS_SPEC_CS, PinList::MASS_SPEC_HEAT};
    HeaterChannelData SamplerHeater = {3, PinList::SAMPLER_CS, PinList::SAMPLER_HEAT};
  } HeaterList;

	const struct SwitchChannelList
	{
		// On-Off
		ChannelData SampleValve = { 0, PinList::J7 };	// Sample Chamber Vacuum Valve
		ChannelData SampleVent = { 1, PinList::J2 };	// Sample Vent Valve
		ChannelData CapilaryValve = { 2, PinList::J1 };	// Capilary Valve
		ChannelData RoughPump = { 3, PinList::J0 };		// Rough pump Falve
		
		// PWM
		ChannelData LampPower = { 4, PinList::J3 };		//Lamp Power via PWM channel 490 Hz
		
		// unused channels
		ChannelData SwitchCh4 = { 5, PinList::J4 };  //PWM channel 490 Hz
		ChannelData SwitchCh5 = { 6, PinList::J5 };  //PWM channel 980 Hz
		ChannelData SwitchCh6 = { 7, PinList::J6 };  //PWM channel 490 Hz
	} SwitchList;

	const struct RtdChannelList
	{
		ChannelData RefTemp0C = { 0, PinList::J10_3 };  // Reference analog channel for temp at 0 degC
		ChannelData CapilaryTemp = { 1, PinList::J8_2 };
		ChannelData SampleChamberTemp = { 3, PinList::J8_6 };
		ChannelData SampleTemp = { 4, PinList::J8_8 };
		ChannelData VacChamberTemp = { 5, PinList::J9_2 };
		
		// unused channels
		ChannelData Ch8_4 = { 2, PinList::J8_4 };
		ChannelData Ch9_4 = { 7, PinList::J9_4 };
		ChannelData Ch9_6 = { 8, PinList::J9_6 };
		ChannelData Ch9_8 = { 9, PinList::J9_8 };
	} RtdList;

	const struct ComControlList
	{
		ChannelData turboTxCtrl = { 0, PinList::TBD_1 };
		ChannelData samplePressTxCtrl = { 1, PinList::TBD_2 };
	} ComCtlList;

	const struct ButtCControlList
	{
		ChannelData butt_1 = { 0, PinList::BUTT_1 };
	} ButtList;
};

