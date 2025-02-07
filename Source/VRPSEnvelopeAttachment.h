/*
  ==============================================================================

    VRPSEnvelopeAttachment.h
    Created: 5 Feb 2025 2:57:52pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include "VRPS.h"
#include "EnvelopeListener.h"
class VRPSEnvelopeAttachment {
    public:
    VRPSEnvelopeAttachment(VRPS* vrps);
    void addListener(EnvelopeListener* envelopeListener);
    void removeListener(EnvelopeListener* envelopeListener);
    private:
    VRPS* vrps;
    EnvelopeListener* envelopeListener;
};
