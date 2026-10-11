import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.Files;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.math.BigInteger;
import java.util.List;
import java.util.Random;
import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;

/** Run original x86 BModel axis-box routines in Ghidra's p-code emulator. */
public class BModelInteractionBoxOracle extends GhidraScript {
 private static final long STACK=0x20010000L,RETURN=0x20018000L,OBJECT=0x30000000L;
 private static final long START=OBJECT+0x100,DIRECTION=OBJECT+0x110,MINIMUM=OBJECT+0x18,MAXIMUM=OBJECT+0x24;
 private static final long NEAR=OBJECT+0x140,FAR=OBJECT+0x144,POINT=OBJECT+0x150,SCORE=OBJECT+0x160;
 private static final int HIT_SENTINEL=0x7fc00001, FAR_SENTINEL=0x7fc00002;
 private static final int[] POINT_SENTINELS={0x7fc00003,0x7fc00004,0x7fc00005};
 private static final int SCORE_SENTINEL=0x7fc00006;

 private EmulatorHelper emu;
 private Address returnAddress;

 private static class Case {
  float[] start,direction,minimum,maximum;
  Case(float[] s,float[] d,float[] lo,float[] hi){start=s.clone();direction=d.clone();minimum=lo.clone();maximum=hi.clone();}
 }

 @Override protected void run() throws Exception {
  String[] args=getScriptArgs();
  if(args.length!=2){printerr("Usage: BModelInteractionBoxOracle.java <output-json> <original-exe>");return;}
  emu=new EmulatorHelper(currentProgram);
  try {
   returnAddress=address(RETURN);emu.setBreakpoint(returnAddress);
   setInitialX87State();
   List<Case> cases=buildCases();
   StringBuilder json=new StringBuilder(160000);
   json.append("{\n  \"provenance\":{\"exe_sha256\":\"").append(exeHash(args[1]))
       .append("\",\"entries\":[\"00482B90\",\"00489D80\"],\"engine\":\"Ghidra EmulatorHelper x86 p-code\",\"x87_control_word\":\"0x037f\",\"x87_status_word\":\"0x0000\",\"method\":\"Original x86 ray/AABB and BModel axis-box query; finite inputs; untouched output sentinels retained on misses\",\"pcode_signed_zero_artifact_samples\":[16,18,28,38,40,50,60,62,72]},\n  \"samples\":[\n");
   for(int i=0;i<cases.size();i++){
    if(monitor.isCancelled())return;
    Case c=cases.get(i);int[] result=capture(c);
    json.append("    {\"start\":");appendVector(json,c.start);
    json.append(",\"direction\":");appendVector(json,c.direction);
    json.append(",\"minimum\":");appendVector(json,c.minimum);
    json.append(",\"maximum\":");appendVector(json,c.maximum);
    json.append(",\"box_hit\":").append(Integer.toUnsignedLong(result[0]));
    json.append(",\"near_bits\":").append(Integer.toUnsignedLong(result[1]));
    json.append(",\"far_bits\":").append(Integer.toUnsignedLong(result[2]));
    json.append(",\"query_hit\":").append(Integer.toUnsignedLong(result[3]));
    json.append(",\"point_bits\":[").append(Integer.toUnsignedLong(result[4])).append(',').append(Integer.toUnsignedLong(result[5])).append(',').append(Integer.toUnsignedLong(result[6])).append(']');
    json.append(",\"score_bits\":").append(Integer.toUnsignedLong(result[7]));
    if(isPcodeSignedZeroArtifact(i))json.append(",\"pcode_signed_zero_artifact\":true");
    json.append('}');
    json.append(i+1==cases.size()?"\n":" ,\n");
   }
   json.append("  ]\n}\n");
   File output=new File(args[0]);Files.writeString(output.toPath(),json.toString(),StandardCharsets.UTF_8);
   println("Captured "+cases.size()+" original BModel ray/AABB queries in "+output.getAbsolutePath());
  }
  finally {emu.dispose();}
 }

 private void setInitialX87State() throws Exception {
  for(String name:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(name,0);
  Register control=currentProgram.getLanguage().getRegister("FPUControlWord");
  if(control==null)throw new IllegalStateException("Ghidra language has no FPUControlWord register");
  emu.writeRegister(control,0x037fL);
  Register status=currentProgram.getLanguage().getRegister("FPUStatusWord");
  if(status==null)throw new IllegalStateException("Ghidra language has no FPUStatusWord register");
  emu.writeRegister(status,0L);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");
  if(tag!=null)emu.writeRegister(tag,0xffffL);
  for(Register register:currentProgram.getLanguage().getRegisters()){
   String name=register.getName().toUpperCase();
   if(name.matches("ST[0-7]"))emu.writeRegister(register,BigInteger.ZERO);
  }
 }

 private int[] capture(Case c) throws Exception {
  writeVector(START,c.start);writeVector(DIRECTION,c.direction);writeVector(MINIMUM,c.minimum);writeVector(MAXIMUM,c.maximum);
  writeWord(NEAR,HIT_SENTINEL);writeWord(FAR,FAR_SENTINEL);
  call(0x482b90L,new long[]{START,DIRECTION,MINIMUM,MAXIMUM,NEAR,FAR},0);
  int hit=(int)(emu.readRegister("EAX").longValue()&0xff),near=readWord(NEAR),far=readWord(FAR);
  for(int i=0;i<3;i++)writeWord(POINT+4*i,POINT_SENTINELS[i]);writeWord(SCORE,SCORE_SENTINEL);
  call(0x489d80L,new long[]{START,DIRECTION,POINT,SCORE},OBJECT);
  return new int[]{hit,near,far,(int)(emu.readRegister("EAX").longValue()&0xff),
   readWord(POINT),readWord(POINT+4),readWord(POINT+8),readWord(SCORE)};
 }

 private void call(long entry,long[] args,long self) throws Exception {
  Address sp=address(STACK);emu.writeMemoryValue(sp,4,RETURN);
  for(int i=0;i<args.length;i++)emu.writeMemoryValue(sp.add(4L+4L*i),4,args[i]);
  emu.writeRegister("ESP",STACK);if(self!=0)emu.writeRegister("ECX",self);
  emu.writeRegister(emu.getPCRegister(),entry);
  if(!emu.run(monitor))throw new IllegalStateException("Emulation error at "+emu.getExecutionAddress()+": "+emu.getLastError());
  if(!emu.getExecutionAddress().equals(returnAddress))throw new IllegalStateException("Routine "+Long.toHexString(entry)+" stopped at "+emu.getExecutionAddress());
 }

 private void writeVector(long at,float[] values) throws Exception {for(int i=0;i<3;i++)writeWord(at+4L*i,Float.floatToRawIntBits(values[i]));}
 private void writeWord(long at,int value) throws Exception {emu.writeMemoryValue(address(at),4,Integer.toUnsignedLong(value));}
 private int readWord(long at) throws Exception {byte[] b=emu.readMemory(address(at),4);return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private Address address(long value){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(value);}
 private static void appendVector(StringBuilder out,float[] values){out.append('[').append(Float.toString(values[0])).append(',').append(Float.toString(values[1])).append(',').append(Float.toString(values[2])).append(']');}

 private String exeHash(String path) throws Exception {
  MessageDigest digest=MessageDigest.getInstance("SHA-256");digest.update(Files.readAllBytes(Path.of(path)));
  StringBuilder value=new StringBuilder();for(byte b:digest.digest())value.append(String.format("%02x",b&255));return value.toString();
 }

 private static List<Case> buildCases(){
  List<Case> cases=new ArrayList<>();float[] lo={-1,-1,-1},hi={1,1,1};
  add(cases,new float[]{-3,0,0},new float[]{1,0,0},lo,hi);add(cases,new float[]{-3,0,0},new float[]{-1,0,0},lo,hi);
  add(cases,new float[]{0,0,0},new float[]{0,0,0},lo,hi);add(cases,new float[]{2,0,0},new float[]{0,1,0},lo,hi);
  add(cases,new float[]{1,0,0},new float[]{0,1,0},lo,hi);add(cases,new float[]{-1,0,0},new float[]{0,-1,0},lo,hi);
  add(cases,new float[]{-3,2,0},new float[]{1,0,0},lo,hi);add(cases,new float[]{-3,0,0},new float[]{1,1,0},lo,hi);
  add(cases,new float[]{-3,2,0},new float[]{1,-1,0},lo,hi);add(cases,new float[]{3,3,3},new float[]{-1,-1,-1},lo,hi);
  float threshold=1.0e-28f;float[] near={threshold,nextFloat(threshold,false),nextFloat(threshold,true),-threshold,nextFloat(-threshold,false),nextFloat(-threshold,true),1.0e-29f,-1.0e-29f,1.0e-27f,-1.0e-27f,0};
  for(int axis=0;axis<3;axis++)for(float direction:near){float[] start={0,0,0},ray={0,0,0};start[axis]=-1;ray[axis]=direction;add(cases,start,ray,lo,hi);start[axis]=2;ray[axis]=-direction;add(cases,start,ray,lo,hi);}
  Random random=new Random(48290);
  for(int n=0;n<192;n++){
   float[] center=new float[3],extent=new float[3],minimum=new float[3],maximum=new float[3],start=new float[3],direction=new float[3];
   for(int axis=0;axis<3;axis++){center[axis]=f32((random.nextDouble()*4000)-2000);extent[axis]=f32(.01+random.nextDouble()*180);minimum[axis]=f32(center[axis]-extent[axis]);maximum[axis]=f32(center[axis]+extent[axis]);start[axis]=f32((random.nextDouble()*5000)-2500);direction[axis]=random.nextInt(4)==0?0:f32((random.nextDouble()*4)-2);}
   add(cases,start,direction,minimum,maximum);
  }
  return cases;
 }
 private static void add(List<Case> cases,float[] s,float[] d,float[] lo,float[] hi){cases.add(new Case(s,d,lo,hi));}
 private static float f32(double value){return (float)value;}
 private static float nextFloat(float value,boolean up){int bits=Float.floatToRawIntBits(value);if(value==0)return Float.intBitsToFloat(up?1:0x80000001);bits+=((value>0)==up)?1:-1;return Float.intBitsToFloat(bits);}
 private static boolean isPcodeSignedZeroArtifact(int sampleIndex){
  // EmulatorHelper leaves stale x87 C0 on these signed-zero boundary cases.
  return sampleIndex==16||sampleIndex==18||sampleIndex==28||sampleIndex==38||sampleIndex==40||sampleIndex==50||sampleIndex==60||sampleIndex==62||sampleIndex==72;
 }
}
