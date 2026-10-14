/* Fresh Ghidra decompilation: FDNYFirefighter.exe
   SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
   Ghidra address: 00482AA0. Retail roam caller: 0045B220 at 0045B34B. */

float10 __cdecl ExponentialHeadingBlend(float oldHeading,
                                        float desiredHeading)
{
  float10 oldWeight;

  /* x87 inputs to the CRT power helper are base=0.5 and exponent=frameDelta/0.3. */
  oldWeight = (float10)pow(0.5, DAT_00502b04 / 0.3f);
  return oldWeight * (float10)oldHeading +
         ((float10)1.0 - oldWeight) * (float10)desiredHeading;
}

/* Relevant instruction/data evidence:
   00482AA0 FLD double ptr [004DE6F8]  ; bytes encode double 0.5
   00482AA6 FLD float ptr  [00502B04]  ; shared frame delta
   00482AAC FDIV float ptr  [ESP+0x0C]; caller 0045B220 pushes 0x3E99999A (0.3f)
   00482AB0 CALL 004B9A20              ; CRT x87 pow adapter to 004B9A7D
   00482AB5 FLD float ptr  [004DFC24]  ; 1.0
   00482ABB FSUB ST0,ST1               ; 1.0 - oldWeight
   00482ABD FMUL float ptr  [ESP+0x08]; desired heading
   00482AC1 FXCH
   00482AC3 FMUL float ptr  [ESP+0x04]; old heading
   00482AC7 FADDP

   Caller 0045B220 first unwraps the desired angle to the nearest equivalent
   heading. The shared port supplies its rendered-frame delta; retail callback
   cadence remains unverified. */
