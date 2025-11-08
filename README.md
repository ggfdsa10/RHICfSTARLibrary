RHICfSTARLibrary
================

RHICf Reconstruction and Simulation library under the STAR software [star-sw](https://github.com/star-bnl/star-sw, "star software")

# Reconstruction tool

* **StRHICfRawHitMaker** : Data conversion tool for StRoot format
* **StRHICfHitMaker** : RHICf calibration main tool
* **StRHICfPointMaker** : RHICf Reconstruction main tool

## Reconstruction Utility tool

* **StRHICfDbMaker** : RHICf database tool

* **StRHICfUtil**
  * **StRHICfPID** : RHICf Particle identification tool
  * **StRHICfRecoPos** : RHICf particle position reconstruction tool
  * **StRHICfRecoEnergy** : RHICf particle energy reconstruction tool
  * **StRHICfFunction** : A useful functions class for reconstruction

## RHICf data format in StRoot (StEvent/StMuDst)
* **StRHICfRawHit/StMuRHICfRawHit** : raw RHICf data
* **StRHICfHit/StMuRHICfHit** : Reconstructed RHICf detector-level data
* **StRHICfPoint/StMuRHICfPoint** : RHICf Particle-level data

## Analysis RHICf data format (StRHICfEventDst)
* **StRHICfPool/StRHICfEventDst** : New RHICf analysis data format
* **StRHICfPool/StRHICfEventMaker** : Merge the RHICf and STAR MuDst-level data into a new format

## Reconstruction macro
* **root4star_macro/makeRHICfEvent.C** : A full RHICf reconstruction and conversion tool into StRHICfEventDst.
* **root4star_macro/readRHICfEvent.C** : Simple StRHICfEventDst reader

# Simulation tool

* **StRHICfPool/StRHICfSimDst** : RHICf+STAR simulation data format
* **StRHICfPool/StRHICfSimGenerator** : RHICf private generator (CRMC) conversion into STAR simulation
* **StRHICfPool/StRHICfSimConvertor** : Conversion tool from STAR simulation data to RHICf+STAR simulation

## Simulation generate macro
* **root4star_macro/StarSimulationGenerate.C** : STAR parts simulation with generators
* **root4star_macro/ConvertStarSimToRHICfSimDst.C** : STAR simulation conversion into RHICfSimDst format
* **root4star_macro/RHICfSimulationReconstruction.C** : RHICf+STAR simulation reconstruction for RHICf detector

