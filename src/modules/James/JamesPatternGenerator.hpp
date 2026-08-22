#pragma once
#include <rack.hpp>

// Genre-based probabilistic drum pattern generator.
// Adapted from the "Beat Agent" concept and extended with:
// - Fixed step patterns + probability (P)
// - Euclidean sequencing (E(k,n)) for percussion
// - Global choke (Open HH silences Closed HH for ALL genres)
// - Markov chain style (transitions between instruments)
// - Ratcheting (32nd-note subdivisions for Trap)
// - Microtiming jitter (Glitch / Braindance)
//
// Row order: 0=KICK, 1=SNARE, 2=HHCL, 3=HHABT, 4=PERC1, 5=PERC2

struct GenreRule
{
	const char *name;
	float probs[6][16]; // probability 0.0 - 1.0 per row and step
	int euclidK;        // Euclidean: k hits
	int euclidN;        // Euclidean: n steps (0 = no euclidean)
	bool useMarkov;     // If true, use the Markov transition matrix instead of probs
	bool has_ratcheting; // If true, HHCL gets 32nd-note ratchets on steps 3,7,11,15
	float jitterAmount; // Microtiming jitter in fraction of step (0.0 = none)
};

// Euclidean pattern (Bjorklund algorithm): returns true if step is a hit
// for k hits distributed evenly across n steps.
inline bool euclideanPattern(int k, int n, int step)
{
	if (k <= 0 || n <= 0)
		return false;
	if (k >= n)
		return true;
	// Standard Bjorklund distribution
	int bucket = 0;
	for (int i = 0; i < n; i++)
	{
		bucket += k;
		if (bucket >= n)
		{
			if (i == step)
				return true;
			bucket -= n;
		}
	}
	return false;
}

// 11 genres. Row order: 0=KICK, 1=SNARE, 2=HHCL, 3=HHABT, 4=PERC1, 5=PERC2
static const GenreRule GEN_RES[] = {
	// House
	{
		"House",
		{
			// KICK: four-on-the-floor
			{1.00f, 0.05f, 0.05f, 0.10f, 1.00f, 0.05f, 0.05f, 0.10f, 1.00f, 0.05f, 0.05f, 0.10f, 1.00f, 0.05f, 0.15f, 0.05f},
			// SNARE: firm backbeat 90-100% on 4,12; ghosts 10-20% on 7,10,14
			{0.00f, 0.00f, 0.00f, 0.00f, 0.95f, 0.00f, 0.00f, 0.15f, 0.00f, 0.00f, 0.10f, 0.00f, 0.95f, 0.00f, 0.20f, 0.00f},
			// HHCL: constant
			{0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f},
			// HHABT: open on offbeats
			{0.00f, 0.00f, 0.95f, 0.00f, 0.00f, 0.00f, 0.95f, 0.00f, 0.00f, 0.00f, 0.95f, 0.00f, 0.00f, 0.00f, 0.95f, 0.00f},
			// PERC1: accent on 4,12
			{0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f},
			// PERC2: syncopated offsets
			{0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.20f},
		},
		3, 16, false, false, 0.0f,
	},
	// Techno
	{
		"Techno",
		{
			// KICK: four-on-the-floor with ghost variation
			{1.00f, 0.05f, 0.10f, 0.05f, 1.00f, 0.10f, 0.20f, 0.05f, 1.00f, 0.05f, 0.10f, 0.05f, 1.00f, 0.10f, 0.30f, 0.10f},
			// SNARE: firm backbeat 90% on 4,8,12; ghosts 15% on 7,14
			{0.00f, 0.00f, 0.00f, 0.00f, 0.90f, 0.00f, 0.00f, 0.15f, 0.90f, 0.00f, 0.05f, 0.00f, 0.90f, 0.00f, 0.15f, 0.00f},
			// HHCL: dense
			{0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.80f},
			// HHABT: open on offbeats
			{0.00f, 0.00f, 0.85f, 0.00f, 0.00f, 0.00f, 0.85f, 0.00f, 0.00f, 0.00f, 0.85f, 0.00f, 0.00f, 0.00f, 0.85f, 0.00f},
			// PERC1: accent
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.10f, 0.00f},
			// PERC2: 8th clave
			{0.00f, 0.40f, 0.00f, 0.20f, 0.00f, 0.40f, 0.00f, 0.20f, 0.00f, 0.40f, 0.00f, 0.20f, 0.00f, 0.40f, 0.20f, 0.20f},
		},
		7, 16, false, false, 0.0f,
	},
	// Trap
	{
		"Trap",
		{
			// KICK: syncopated
			{1.00f, 0.00f, 0.10f, 0.00f, 0.20f, 0.00f, 0.40f, 0.70f, 0.10f, 0.50f, 0.00f, 0.70f, 0.30f, 0.00f, 0.10f, 0.20f},
			// SNARE: on step 8 only (trap snare on 3)
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// HHCL: roll with ratchets on 3,7,11,15
			{0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f},
			// HHABT: sparse
			{0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC1: accent on 8
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC2: sparse accents
			{0.00f, 0.00f, 0.00f, 0.25f, 0.00f, 0.00f, 0.00f, 0.25f, 0.00f, 0.00f, 0.00f, 0.35f, 0.00f, 0.30f, 0.30f, 0.30f},
		},
		0, 0, false, true, 0.0f,
	},
	// Drum & Bass
	{
		"Drum & Bass",
		{
			// KICK: 2-step pattern (hits on 0 and 10 only)
			{1.00f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.10f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// SNARE: 100% on 4 and 12
			{0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.10f, 0.20f, 0.00f, 0.30f, 0.00f, 0.10f, 0.00f, 1.00f, 0.10f, 0.30f, 0.30f},
			// HHCL: 2-step break
			{0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.30f},
			// HHABT: open accents
			{0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC1: single accent
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f},
			// PERC2: 2-step break ghost notes
			{0.00f, 0.00f, 0.30f, 0.00f, 0.30f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f, 0.00f, 0.40f, 0.20f, 0.30f, 0.00f, 0.30f},
		},
		0, 0, false, false, 0.0f,
	},
	// Minimal
	{
		"Minimal",
		{
			// KICK: forced four-on-the-floor 100% on 0,4,8,12
			{1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f},
			// SNARE: backbeat subtle
			{0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f},
			// HHCL: sparse
			{0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.20f},
			// HHABT: sparse open
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.25f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.25f},
			// PERC1: very sparse
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC2: alternating pulse
			{0.20f, 0.30f, 0.00f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f},
		},
		3, 16, false, false, 0.0f,
	},
	// Afro
	{
		"Afro",
		{
			// KICK: tresillo pattern (hits on 0,3,6,10,12) using euclidean 3/8
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// SNARE: accents
			{0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.10f, 0.00f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.30f, 0.00f, 0.10f, 0.00f},
			// HHCL: offbeats
			{0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f},
			// HHABT: open accents
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC1: strong accents
			{0.00f, 0.00f, 0.00f, 0.00f, 0.75f, 0.00f, 0.00f, 0.75f, 0.00f, 0.00f, 0.00f, 0.00f, 0.75f, 0.00f, 0.00f, 0.00f},
			// PERC2: obstinato
			{0.25f, 0.00f, 0.35f, 0.00f, 0.25f, 0.00f, 0.00f, 0.35f, 0.00f, 0.25f, 0.00f, 0.35f, 0.00f, 0.25f, 0.00f, 0.30f},
		},
		3, 8, false, false, 0.0f, // euclid 3/8 applied to KICK (row 0)
	},
	// Electro
	{
		"Electro",
		{
			// KICK: electro pattern
			{0.90f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.90f, 0.00f, 0.00f, 0.00f, 0.90f, 0.00f, 0.00f, 0.00f, 0.90f, 0.00f},
			// SNARE: backbeat
			{0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.20f, 0.00f},
			// HHCL: 8ths
			{0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.40f},
			// HHABT: open
			{0.00f, 0.00f, 0.00f, 0.70f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.70f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC1: euclidean 5/16
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC2: funky upbeats
			{0.35f, 0.00f, 0.00f, 0.35f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f, 0.35f, 0.00f, 0.00f, 0.40f, 0.00f, 0.35f, 0.00f},
		},
		5, 16, false, false, 0.0f, // euclid 5/16 applied to PERC1 (row 4)
	},
	// Ambient
	{
		"Ambient",
		{
			{0.10f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.05f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.05f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.20f, 0.30f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f},
			// PERC1: sparse
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC2: sparse texture
			{0.00f, 0.05f, 0.00f, 0.00f, 0.00f, 0.05f, 0.00f, 0.00f, 0.00f, 0.05f, 0.00f, 0.00f, 0.00f, 0.05f, 0.00f, 0.05f},
		},
		0, 0, false, false, 0.0f,
	},
	// Glitch / Micro-editing
	{
		"Glitch",
		{
			{0.50f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f},
			{0.40f, 0.00f, 0.40f, 0.00f, 0.40f, 0.00f, 0.40f, 0.00f, 0.40f, 0.00f, 0.40f, 0.00f, 0.40f, 0.00f, 0.40f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.20f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f},
			// PERC2: irregular spikes
			{0.00f, 0.10f, 0.00f, 0.25f, 0.00f, 0.10f, 0.00f, 0.25f, 0.00f, 0.10f, 0.30f, 0.00f, 0.00f, 0.25f, 0.10f, 0.35f},
		},
		0, 0, false, false, 0.005f, // 5ms jitter
	},
	// Braindance
	{
		"Braindance",
		{
			{0.90f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.10f, 0.50f, 0.20f, 0.10f, 0.60f, 0.85f, 0.10f, 0.00f, 0.20f, 0.10f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.90f, 0.10f, 0.20f, 0.00f, 0.30f, 0.00f, 0.10f, 0.00f, 0.90f, 0.10f, 0.30f, 0.30f},
			{0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.30f},
			{0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f},
			// PERC2: chaotic fills
			{0.00f, 0.15f, 0.30f, 0.10f, 0.30f, 0.00f, 0.40f, 0.10f, 0.00f, 0.30f, 0.15f, 0.40f, 0.00f, 0.30f, 0.20f, 0.40f},
		},
		0, 0, false, false, 0.010f, // 10ms jitter
	},
	// Generative / Markovian
	{
		"Markovian",
		{
			{0.30f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f},
			{0.30f, 0.00f, 0.30f, 0.00f, 0.30f, 0.00f, 0.30f, 0.00f, 0.30f, 0.00f, 0.30f, 0.00f, 0.30f, 0.00f, 0.30f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.20f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f},
			// PERC2: sparse ghost via matrix
			{0.05f, 0.10f, 0.05f, 0.10f, 0.05f, 0.10f, 0.05f, 0.10f, 0.05f, 0.10f, 0.05f, 0.10f, 0.05f, 0.10f, 0.05f, 0.10f},
		},
		0, 0, true, false, 0.0f,
	},
};

static const int NUM_GENRES = 11;

// Markov transition matrix (6x6): probability of instrument j firing
// given instrument i fired on the previous step.
// Row order: 0=KICK, 1=SNARE, 2=HHCL, 3=HHABT, 4=PERC1, 5=PERC2
static const float MARKOV_MATRIX[6][6] = {
	// from KICK
	{0.0f, 0.7f, 0.2f, 0.0f, 0.0f, 0.1f},
	// from SNARE
	{0.1f, 0.0f, 0.0f, 0.6f, 0.3f, 0.0f},
	// from HHCL
	{0.2f, 0.1f, 0.0f, 0.3f, 0.1f, 0.3f},
	// from HHABT
	{0.3f, 0.2f, 0.4f, 0.0f, 0.0f, 0.1f},
	// from PERC1
	{0.4f, 0.3f, 0.1f, 0.0f, 0.0f, 0.2f},
	// from PERC2
	{0.2f, 0.1f, 0.3f, 0.1f, 0.1f, 0.2f},
};

// Generate a 6x16 pattern (0.0 or 1.0) from a genre rule.
// Applies probability, euclidean (kick + percussion), global choke,
// ratcheting, markov, and microtiming jitter.
inline void generatePatternForGenre(const GenreRule &rule, float out[6][16])
{
	// Precompute Euclidean masks once instead of recomputing per cell.
	bool euclidMaskKick[16] = {};
	bool euclidMaskPerc[16] = {};
	int euclidOffsetKick = 0;
	int euclidOffsetPerc = 0;

	// Euclidean for KICK (row 0) — used by Afro (3/8)
	if (rule.euclidN > 0)
	{
		euclidOffsetKick = (int)(random::uniform() * 16);
		for (int i = 0; i < 16; i++)
			euclidMaskKick[i] = euclideanPattern(rule.euclidK, rule.euclidN, i);
	}

	// Euclidean for PERC2 (row 5) — legacy usage (House, Techno, Minimal)
	// Reuse same euclid params but with separate offset for variety
	if (rule.euclidN > 0)
	{
		euclidOffsetPerc = (int)(random::uniform() * 16);
		for (int i = 0; i < 16; i++)
			euclidMaskPerc[i] = euclideanPattern(rule.euclidK, rule.euclidN, i);
	}

	// Track which instrument fired on the previous step for Markov
	int prevInstrument = -1;

	for (int col = 0; col < 16; col++)
	{
		for (int row = 0; row < 6; row++)
		{
			float p = rule.probs[row][col];

			// Markov: use transition probability from previous instrument
			if (rule.useMarkov && prevInstrument >= 0)
			{
				p = MARKOV_MATRIX[prevInstrument][row];
			}

			// Euclidean: override row with euclidean pattern
			if (rule.euclidN > 0)
			{
				if (row == 0)
				{
					// KICK euclidean (Afro tresillo)
					out[row][col] = euclidMaskKick[(col + euclidOffsetKick) % 16] ? 1.0f : 0.0f;
					continue;
				}
				if (row == 4 && rule.euclidK == 5 && rule.euclidN == 16)
				{
					// PERC1 euclidean (Electro 5/16)
					out[row][col] = euclidMaskPerc[(col + euclidOffsetPerc) % 16] ? 1.0f : 0.0f;
					continue;
				}
				if (row == 5 && rule.euclidK != 5)
				{
					// PERC2 euclidean (legacy: House 3/16, Techno 7/16, Minimal 3/16)
					out[row][col] = euclidMaskPerc[(col + euclidOffsetPerc) % 16] ? 1.0f : 0.0f;
					continue;
				}
			}

			out[row][col] = (random::uniform() < p) ? 1.0f : 0.0f;
		}

		// Global choke: if Open HH (row 3) is active, silence Closed HH (row 2)
		// Applied unconditionally for ALL genres
		if (out[3][col] > 0.5f)
		{
			out[2][col] = 0.0f;
		}

		// Ratcheting: if enabled, duplicate HHCL on steps 3,7,11,15 as 32nd-note flam
		// This is a metadata marker — the actual 32nd-note subdivision is handled
		// by the clock engine reading the pattern. We mark by setting a flag value.
		if (rule.has_ratcheting && (col == 3 || col == 7 || col == 11 || col == 15))
		{
			// Ratchet indicator: store 2.0f to signal double-trigger on this step
			// The clock engine will fire HHCL twice (early + on-beat) for this step.
			if (out[2][col] > 0.5f)
				out[2][col] = 2.0f; // ratchet marker
		}

		// Determine which instrument fired this step (for Markov)
		prevInstrument = -1;
		for (int row = 0; row < 6; row++)
		{
			if (out[row][col] > 0.5f)
			{
				prevInstrument = row;
				break;
			}
		}
	}

	// Ensure at least one kick in row 0 (if the genre produced none)
	bool hasKick = false;
	for (int col = 0; col < 16; col++)
	{
		if (out[0][col] > 0.5f)
		{
			hasKick = true;
			break;
		}
	}
	if (!hasKick)
	{
		out[0][0] = 1.0f;
	}
}