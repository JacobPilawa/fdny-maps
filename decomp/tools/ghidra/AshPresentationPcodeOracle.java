import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Executes the original Training ash descriptor presentation helpers. */
public class AshPresentationPcodeOracle extends GhidraScript {
 private static final long OBJECT=0x21000000L,FRAMES=0x21001000L,ARRAY=0x21002000L;
 private static final long CONTAINER=0x21003000L,OUTPUT=0x21004000L,STACK=0x21005000L;
 private static final long TRAMP=0x21005100L,RETURN=0x70000000L,LIST_BOUNDS=0x0048d490L;
 private static final long COLOR=0x0047d420L,BYTES=0x0047d6d0L,ALPHA=0x0047d8a0L;
 private static final int[][] TRAINING={
  {0x3f000000,0x3f800000,0x3e4ccccd,0x3f666666,0x3dcccccd,0x00000000},
  {0x3f000000,0x3f800000,0x3e4ccccd,0x3f666666,0x3f000000,0x00000000},
  {0x3f000000,0x3f800000,0x3e4ccccd,0x3f000000,0x3f000000,0x3f000000},
  {0x3f800000,0x3f800000,0x3e4ccccd,0x00000000,0x00000000,0x00000000},
  {0x3f800000,0x00000000,0x3e4ccccd,0x00000000,0x00000000,0x00000000}
 };
 private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
 private void w(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(EmulatorHelper e,long p)throws Exception{byte[]b=e.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private long q(EmulatorHelper e,long p)throws Exception{return Integer.toUnsignedLong(r(e,p))|(Integer.toUnsignedLong(r(e,p+4))<<32);}
 private void cpu(EmulatorHelper e)throws Exception{
  for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})e.writeRegister(n,0);
  e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);
  Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)e.writeRegister(cw,0x037fL);
  Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)e.writeRegister(sw,0);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)e.writeRegister(tag,0xffffL);
  for(Register reg:currentProgram.getLanguage().getRegisters())if(reg.getName().toUpperCase().matches("ST[0-7]"))e.writeRegister(reg,BigInteger.ZERO);
 }
 private void run(EmulatorHelper e,long entry,long ecx,int[] args,long retStop)throws Exception{
  cpu(e);w(e,STACK,(int)retStop);for(int i=0;i<args.length;i++)w(e,STACK+4L*(i+1),args[i]);
  e.writeRegister("ESP",STACK);e.writeRegister("ECX",ecx);e.getEmulator().setExecuteAddress(entry);
  for(int n=0;n<100000;n++){
   long pc=e.getExecutionAddress().getOffset();if(pc==retStop)return;
   if(pc==LIST_BOUNDS){long sp=e.readRegister("ESP").longValue();long ret=Integer.toUnsignedLong(r(e,sp));e.writeRegister("ESP",sp+8);e.writeRegister("EAX",1);e.getEmulator().setExecuteAddress(ret);continue;}
   if(!e.step(monitor))throw new IllegalStateException("Ash presentation P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
  }
  throw new IllegalStateException("Ash presentation instruction limit at "+e.getExecutionAddress());
 }
 private static final class Case{
  final String name;final int flags,current,next,now,timestamp,rate,scale,opacity;
  final int alphaCurrent,alphaNext;
  Case(String n,int f,int c,int x,int now,int start,int rate,int scale,int opacity,int ac,int an){name=n;flags=f;current=c;next=x;this.now=now;timestamp=start;this.rate=rate;this.scale=scale;this.opacity=opacity;alphaCurrent=ac;alphaNext=an;}
 }
 private String sample(Case s)throws Exception{
  EmulatorHelper e=new EmulatorHelper(currentProgram);
  try{
   e.writeMemory(a(OBJECT),new byte[0x100]);e.writeMemory(a(FRAMES),new byte[0x100]);
   e.writeMemory(a(ARRAY),new byte[0x100]);e.writeMemory(a(CONTAINER),new byte[0x20]);
   e.writeMemory(a(OUTPUT),new byte[0x40]);e.writeMemory(a(STACK),new byte[0x200]);
   w(e,OBJECT+4,s.current);w(e,OBJECT+8,s.next);w(e,OBJECT+0xc,(int)CONTAINER);
   w(e,OBJECT+0x14,s.timestamp);w(e,OBJECT+0x18,s.flags);w(e,OBJECT+0x1c,s.rate);
   w(e,OBJECT+0x20,s.scale);w(e,OBJECT+0x80,s.opacity);w(e,0x00502afcL,s.now);
   w(e,CONTAINER,(int)ARRAY);
   for(int i=0;i<TRAINING.length;i++){
    long frame=FRAMES+0x20L*i;w(e,ARRAY+4L*i,(int)frame);
    for(int j=0;j<6;j++)w(e,frame+4L*(j+1),TRAINING[i][j]);
   }
   // One non-Training control proves the alpha interpolation operands are not
   // accidentally specialized to Training's constant 0.2 values.
   w(e,FRAMES+0x20L*s.current+12,s.alphaCurrent);
   w(e,FRAMES+0x20L*s.next+12,s.alphaNext);
   run(e,COLOR,OBJECT,new int[0],RETURN);
   long colorPtr=Integer.toUnsignedLong((int)e.readRegister("EAX").longValue());
   int[] color={r(e,colorPtr),r(e,colorPtr+4),r(e,colorPtr+8)};
   run(e,BYTES,OBJECT,new int[]{(int)OUTPUT,(int)(OUTPUT+1)},RETURN);
   int byteA=r(e,OUTPUT)&255,byteB=r(e,OUTPUT+1)&255;
   // Preserve the helper's extended return as a binary64 diagnostic, then
   // follow the real 0047F960 caller tail: opacity FMUL and binary32 spill.
   byte[] code=new byte[21];int k=0;code[k++]=(byte)0xd9;code[k++]=(byte)0xc0;
   code[k++]=(byte)0xdd;code[k++]=(byte)0x1d;put32(code,k,(int)(OUTPUT+4));k+=4;
   code[k++]=(byte)0xd8;code[k++]=(byte)0x0d;put32(code,k,(int)(OBJECT+0x80));k+=4;
   code[k++]=(byte)0xd9;code[k++]=(byte)0x1d;put32(code,k,(int)(OUTPUT+12));k+=4;code[k++]=(byte)0xc3;
   e.writeMemory(a(TRAMP),code);
   run(e,ALPHA,OBJECT,new int[0],TRAMP);
   // ALPHA returned to TRAMP. run() stops on the trampoline address initially,
   // so execute its original-caller alpha multiply/spill tail to the sentinel.
   w(e,STACK+4,(int)RETURN);e.getEmulator().setExecuteAddress(TRAMP);e.writeRegister("ESP",STACK+4);
   for(int n=0;n<1000&&e.getExecutionAddress().getOffset()!=RETURN;n++)
    if(!e.step(monitor))throw new IllegalStateException("Ash alpha-tail P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
   if(e.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Ash alpha-tail did not return");
   long alphaDouble=q(e,OUTPUT+4);int scaledAlpha=r(e,OUTPUT+12);
   return "{\"name\":\""+s.name+"\",\"flags\":"+Integer.toUnsignedLong(s.flags)+",\"current\":"+s.current+",\"next\":"+s.next+",\"now\":"+Integer.toUnsignedLong(s.now)+",\"timestamp\":"+Integer.toUnsignedLong(s.timestamp)+",\"rate_bits\":"+Integer.toUnsignedLong(s.rate)+",\"byte_scale_bits\":"+Integer.toUnsignedLong(s.scale)+",\"opacity_bits\":"+Integer.toUnsignedLong(s.opacity)+",\"current_duration_bits\":"+Integer.toUnsignedLong(r(e,FRAMES+0x20L*s.current+4))+",\"alpha_current_bits\":"+Integer.toUnsignedLong(s.alphaCurrent)+",\"alpha_next_bits\":"+Integer.toUnsignedLong(s.alphaNext)+",\"rgb_bits\":["+Integer.toUnsignedLong(color[0])+","+Integer.toUnsignedLong(color[1])+","+Integer.toUnsignedLong(color[2])+"],\"byte_channels\":["+byteA+","+byteB+"],\"alpha_double_bits\":"+Long.toUnsignedString(alphaDouble)+",\"scaled_alpha_bits\":"+Integer.toUnsignedLong(scaledAlpha)+"}";
  }finally{e.dispose();}
 }
 private void put32(byte[]b,int p,int v){b[p]=(byte)v;b[p+1]=(byte)(v>>>8);b[p+2]=(byte)(v>>>16);b[p+3]=(byte)(v>>>24);}
 private String frameJson(){List<String> rows=new ArrayList<>();for(int[]f:TRAINING)rows.add("{\"duration\":"+Integer.toUnsignedLong(f[0])+",\"byte_channel\":"+Integer.toUnsignedLong(f[1])+",\"alpha\":"+Integer.toUnsignedLong(f[2])+",\"color\":["+Integer.toUnsignedLong(f[3])+","+Integer.toUnsignedLong(f[4])+","+Integer.toUnsignedLong(f[5])+"]}");return String.join(",",rows);}
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshPresentationPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  int one=0x3f800000,half=0x3f000000,threeQuarter=0x3f400000,one125=0x3f900000;
  int alphaOne=0x3e4ccccd;
  Case[] cases={
   new Case("training_record_0_to_1_start",0x3c,0,1,1000,1000,one,one,one,alphaOne,alphaOne),
   new Case("training_record_0_to_1_quarter",0x3c,0,1,1125,1000,one,one,one,alphaOne,alphaOne),
   new Case("training_record_1_to_2_half_scaled",0x3c,1,2,1250,1000,one,half,threeQuarter,alphaOne,alphaOne),
   new Case("training_record_3_to_4_nonunit_rate",0x3c,3,4,1370,1000,one125,one,half,alphaOne,alphaOne),
   new Case("training_clock_wrap",0x3c,1,2,0x20,0xfffffff0,one,one,one,alphaOne,alphaOne),
   new Case("flags_clear",0,0,1,1125,1000,one,one,one,alphaOne,alphaOne),
   new Case("byte_independent_current",0x02,0,1,1125,1000,one,one,one,alphaOne,alphaOne),
   new Case("byte_crossfade_current",0x04,0,1,1125,1000,one,one,one,alphaOne,alphaOne),
   new Case("byte_both_bits",0x06,0,1,1125,1000,one,one,one,alphaOne,alphaOne),
   new Case("rgb_without_alpha",0x2c,1,2,1125,1000,one,one,one,alphaOne,0x3f600000),
   new Case("alpha_interpolation_control",0x10,0,1,1125,1000,one,one,one,0x3e000000,0x3f600000)
  };
  List<String> rows=new ArrayList<>();for(Case c:cases)rows.add(sample(c));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"scope\":\"0047D420 RGB, 0047D6D0 byte channels, and 0047D8A0 alpha with original 004B9400 caller tail; valid five-entry Training record array and controlled object/global inputs; 0048D490 valid-index list bounds stub; one non-Training alpha interpolation pair; x87 reset control 0x037f\",\"frames\":["+frameJson()+"],\"samples\":["+String.join(",",rows)+"]}\n");
  println("Captured "+rows.size()+" original ash presentation cases");
 }
}
