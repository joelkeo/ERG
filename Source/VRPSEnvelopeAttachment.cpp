/*
  ==============================================================================

    VRPSEnvelopeAttachment.cpp
    Created: 5 Feb 2025 2:57:52pm
    Author:  Joel Keohane

  ==============================================================================
*/

#include "VRPSEnvelopeAttachment.h"
VRPSEnvelopeAttachment::VRPSEnvelopeAttachment(VRPS* vrps) {
   this->vrps = vrps;
}
void VRPSEnvelopeAttachment::addListener(EnvelopeListener *envelopeListener) {
    vrps->addListener(envelopeListener);
}
void VRPSEnvelopeAttachment::removeListener(EnvelopeListener* envelopeListener) {
    vrps->removeListener(envelopeListener);
}
