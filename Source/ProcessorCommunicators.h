/*
  ==============================================================================

    ProcessorCommunicators.h
    Created: 5 Feb 2025 3:09:31pm
    Author:  Joel Keohane

  ==============================================================================
*/

#pragma once
#include "VRPSEnvelopeAttachment.h"
#include "ArpPattern.h"
class ProcessorCommunicators {
    public:
    VRPSEnvelopeAttachment& envelopeAttachment;
    ArpPattern& arpPattern;
    ProcessorCommunicators(VRPSEnvelopeAttachment& envelopeAttachment,
                           ArpPattern& arpPattern);
};
