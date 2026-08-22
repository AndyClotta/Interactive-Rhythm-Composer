#pragma once
#include <rack.hpp>

// Genre-based probabilistic drum pattern generator.
// Adapted from the "Beat Agent" concept and extended with:
// - Fixed step patterns + probability (P)
// - Euclidean sequencing (E(k,n)) for percussion
// - Choke logic (Open HH silences Closed HH)
// - Markov chain style (transitions between instruments)
//
// Row order: 0=KICK, 1=SNARE, 2=HHCL, 3=HHABT, 4=PERC1, 5=PERC2

struct GenreRule
{
	const char *name;
	float probs[6][16]; // probability 0.0 - 1.0 per row and step
	int euclidK;        // Euclidean: k hits
	int euclidN;        // Euclidean: n steps (0 = no euclidean)
	bool chokeOpenToClosed; // Open HH silences Closed HH
	bool useMarkov;     // If true, use the Markov transition matrix instead of probs
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
			{1.00f, 0.05f, 0.05f, 0.10f, 1.00f, 0.05f, 0.05f, 0.10f, 1.00f, 0.05f, 0.05f, 0.10f, 1.00f, 0.05f, 0.15f, 0.05f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.10f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.10f, 0.00f},
			{0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f, 0.80f, 0.50f},
			{0.00f, 0.00f, 0.95f, 0.00f, 0.00f, 0.00f, 0.95f, 0.00f, 0.00f, 0.00f, 0.95f, 0.00f, 0.00f, 0.00f, 0.95f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f},
			// PERC2: syncopated offsets
			{0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.30f, 0.00f, 0.20f},
		},
		3, 16, true, false,
	},
	// Techno
	{
		"Techno",
		{
			{1.00f, 0.05f, 0.10f, 0.05f, 1.00f, 0.10f, 0.20f, 0.05f, 1.00f, 0.05f, 0.10f, 0.05f, 1.00f, 0.10f, 0.30f, 0.10f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.15f, 0.00f, 0.00f, 0.00f, 0.15f, 0.00f, 0.05f, 0.00f, 0.15f, 0.00f, 0.00f, 0.00f},
			{0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.70f, 0.90f, 0.80f},
			{0.00f, 0.00f, 0.85f, 0.00f, 0.00f, 0.00f, 0.85f, 0.00f, 0.00f, 0.00f, 0.85f, 0.00f, 0.00f, 0.00f, 0.85f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.10f, 0.00f},
			// PERC2: 8th clave
			{0.00f, 0.40f, 0.00f, 0.20f, 0.00f, 0.40f, 0.00f, 0.20f, 0.00f, 0.40f, 0.00f, 0.20f, 0.00f, 0.40f, 0.20f, 0.20f},
		},
		7, 16, true, false,
	},
	// Trap
	{
		"Trap",
		{
			{1.00f, 0.00f, 0.10f, 0.00f, 0.20f, 0.00f, 0.40f, 0.70f, 0.10f, 0.50f, 0.00f, 0.70f, 0.30f, 0.00f, 0.10f, 0.20f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			{0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f, 0.95f, 0.90f},
			{0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC2: sparse accents
			{0.00f, 0.00f, 0.00f, 0.25f, 0.00f, 0.00f, 0.00f, 0.25f, 0.00f, 0.00f, 0.00f, 0.35f, 0.00f, 0.30f, 0.30f, 0.30f},
		},
		0, 0, false, false,
	},
	// Drum & Bass
	{
		"Drum & Bass",
		{
			{1.00f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.10f, 0.50f, 0.20f, 0.10f, 0.60f, 0.85f, 0.10f, 0.00f, 0.20f, 0.10f},
			{0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.10f, 0.20f, 0.00f, 0.30f, 0.00f, 0.10f, 0.00f, 1.00f, 0.10f, 0.30f, 0.30f},
			{0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.00f, 0.95f, 0.30f},
			{0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC1: single accent
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f},
			// PERC2: 2-step break ghost notes
			{0.00f, 0.00f, 0.30f, 0.00f, 0.30f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f, 0.00f, 0.40f, 0.20f, 0.30f, 0.00f, 0.30f},
		},
		0, 0, false, false,
	},
	// Minimal
	{
		"Minimal",
		{
			{1.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 0.50f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.20f, 0.00f, 0.00f, 0.00f},
			{0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.00f, 0.60f, 0.20f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.25f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.25f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f},
			// PERC2: alternating pulse
			{0.20f, 0.30f, 0.00f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f, 0.00f, 0.30f, 0.00f, 0.20f},
		},
		3, 16, false, false,
	},
	// Afro
	{
		"Afro",
		{
			{0.85f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.70f, 0.10f, 0.60f, 0.00f, 0.10f, 0.00f, 0.10f, 0.00f, 0.10f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.30f, 0.10f, 0.00f, 0.00f, 0.00f, 0.00f, 0.10f, 0.00f, 0.30f, 0.00f, 0.10f, 0.00f},
			{0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f, 0.00f, 0.80f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.60f, 0.00f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.75f, 0.00f, 0.00f, 0.75f, 0.00f, 0.00f, 0.00f, 0.00f, 0.75f, 0.00f, 0.00f, 0.00f},
			// PERC2: obstinato
			{0.25f, 0.00f, 0.35f, 0.00f, 0.25f, 0.00f, 0.00f, 0.35f, 0.00f, 0.25f, 0.00f, 0.35f, 0.00f, 0.25f, 0.00f, 0.30f},
		},
		5, 16, false, false,
	},
	// Electro
	{
		"Electro",
		{
			{0.90f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.90f, 0.00f, 0.00f, 0.00f, 0.90f, 0.00f, 0.00f, 0.00f, 0.90f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.30f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.20f, 0.00f},
			{0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.00f, 0.90f, 0.40f},
			{0.00f, 0.00f, 0.00f, 0.70f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.70f, 0.00f, 0.00f, 0.00f, 0.00f},
			{0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.80f, 0.00f, 0.30f, 0.30f},
			// PERC2: funky upbeats
			{0.35f, 0.00f, 0.00f, 0.35f, 0.00f, 0.00f, 0.40f, 0.00f, 0.00f, 0.35f, 0.00f, 0.00f, 0.40f, 0.00f, 0.35f, 0.00f},
		},
		0, 0, false, false,
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
		0, 0, false, false,
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
		0, 0, false, false,
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
		0, 0, false, false,
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
		0, 0, false, true,
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
// Applies probability, euclidean (percussion), choke, and markov (if genre is Markovian).
inline void generatePatternForGenre(const GenreRule &rule, float out[6][16])
{
	// Precompute the Euclidean mask once instead of recomputing it per cell.
	bool euclidMask[16] = {};
	int euclidOffset = 0;
	if (rule.euclidN > 0)
	{
		// Random phase offset so perc patterns vary while keeping E(k,n) spacing.
		euclidOffset = (int)(random::uniform() * 16);
		for (int i = 0; i < 16; i++)
			euclidMask[i] = euclideanPattern(rule.euclidK, rule.euclidN, i);
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

			// Euclidean: override percussion row with euclidean pattern
			if (row == 5 && rule.euclidN > 0)
			{
				out[row][col] = euclidMask[(col + euclidOffset) % 16] ? 1.0f : 0.0f;
				continue;
			}

			out[row][col] = (random::uniform() < p) ? 1.0f : 0.0f;
		}

		// Choke: if Open HH (row 3) is active, silence Closed HH (row 2)
		if (rule.chokeOpenToClosed && out[3][col] > 0.5f)
		{
			out[2][col] = 0.0f;
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