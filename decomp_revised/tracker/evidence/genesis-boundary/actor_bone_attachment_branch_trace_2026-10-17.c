/* Ghidra pseudocode excerpt / normalized branch summary; names are provisional. */
/* Program: Genesis.dll */
/* Input SHA-256: 5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643 */

/* 0x1000fa60 FUN_1000fa60 */
/* Constants at VA 0x10051d48 and 0x10051d50 are doubles -0.0001 and +0.0001. */
/* Runtime record +0x24 is quaternion (w,x,y,z); flag is +0x84. */

void __fastcall FUN_1000fa60(float *record)
{
  float x = record[0x28 / 4];
  float y = record[0x2c / 4];
  float z = record[0x30 / 4];
  record[0x84 / 4] =
    (-0.0001 < x && x < 0.0001 &&
     -0.0001 < y && y < 0.0001 &&
     -0.0001 < z && z < 0.0001) ? 1 : 0;
}

/* 0x1000fb10 FUN_1000fb10 initializes runtime bone state from the ACT.
   When an attachment exists it calls FUN_1002b730 to convert the ACT matrix
   to quaternion +0x24 and copies 12 matrix floats to +0x34. The attachment
   translation is retained at +0x58..+0x60. Sample translation/quaternion
   fields are cleared, then FUN_1000fa60 evaluates the flag. */

/* 0x1000fd40 FUN_1000fd40, decompiler's two branches (register names omitted):

   if (record->attachmentFlag != 0) {
     currentQuaternion = parentQuaternion * sampledQuaternion;
     currentMatrix = geQuaternion_ToMatrix(currentQuaternion);
     translated = attachmentBaseTranslation + sampledTranslation;
     currentPosition = parentTransform(translated);
   } else {
     currentQuaternion = parentQuaternion * attachmentQuaternion * sampledQuaternion;
     currentMatrix = geQuaternion_ToMatrix(currentQuaternion);
     translated = attachmentMatrix * sampledTranslation;
     translated += attachmentBaseTranslation;
     currentPosition = parentTransform(translated);
   }
*/
