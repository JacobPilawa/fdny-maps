import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;
import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;

/** Scalar-only, read-only p-code probes for BModel throw setup/update. */
public class ThrowScalarOracle extends GhidraScript {
 private static final long STACK=0x20010000L, RETURN=0x20018000L;
 private static final long OBJECT=0x30000000L, ARRAY=0x30001000L, CHILD=0x30002000L;
 private static final long TICK=0x00502afcL;
 private static final long HIT=0x7fc00001L;
 private EmulatorHelper emu;

 @Override protected void run() throws Exception {
  String[] args=getScriptArgs();
  if(args.length!=1){printerr("Usage: ThrowScalarOracle.java <output-json>");return;}
  StringBuilder out=new StringBuilder();
  out.append("{\n  \"provenance\":{\"program\":\"").append(currentProgram.getName())
     .append("\",\"exe_sha256\":\"").append(currentProgram.getExecutableSHA256())
     .append("\",\"engine\":\"Ghidra EmulatorHelper x86 p-code\",\"x87_control_word\":\"0x037f\",\"scope\":\"0043B580 full routine; 0043D330 stopped before 0043BAA0 at active path 0043D522 or completion path 0043D46D; no renderer/event service execution\"},\n  \"setup\":[\n");
  float[][] endpoints={{0,0,0,444,120,0},{-12.5f,25,7.25f,311.75f,-83.5f,22.125f},{2,600,-4,2,-300,4},{0,0,0,480,0,0}};
  int[][] setupRows=new int[endpoints.length][];
  for(int i=0;i<endpoints.length;i++){
   int[] r=setupRows[i]=setup(endpoints[i],0x12345678L+i);
   out.append("    {\"start\":");vec(out,endpoints[i],0);out.append(",\"end\":");vec(out,endpoints[i],3);
   out.append(",\"tick\":").append(0x12345678L+i).append(",\"duration_bits\":").append(Integer.toUnsignedLong(r[0]));
   out.append(",\"acceleration_bits\":").append(Integer.toUnsignedLong(r[1])).append(",\"initial_vertical_velocity_bits\":").append(Integer.toUnsignedLong(r[2]));
   out.append(",\"initial_middle_bits\":").append(Integer.toUnsignedLong(r[3])).append(",\"mode\":").append(r[4]).append("}").append(i+1==endpoints.length?"\n":" ,\n");
  }
  out.append("  ],\n  \"update_input\":{\"setup_case\":3,\"start\":[0.0,0.0,0.0],\"end\":[480.0,0.0,0.0],\"start_tick\":1000,\"duration_bits\":").append(Integer.toUnsignedLong(setupRows[3][0]))
     .append(",\"acceleration_bits\":").append(Integer.toUnsignedLong(setupRows[3][1])).append(",\"initial_vertical_velocity_bits\":").append(Integer.toUnsignedLong(setupRows[3][2])).append("},\n  \"update\":[\n");
  // Use one source-shaped scalar state to probe timer boundaries and direction.
  // Duration is set as a binary32 value, as it is stored by the setup method.
  float[] p0={0,0,0},p1={480,0,0};
  float duration=Float.intBitsToFloat(setupRows[3][0]);
  float accel=Float.intBitsToFloat(setupRows[3][1]);
  float vy=Float.intBitsToFloat(setupRows[3][2]);
  int[] deltas={0,1,499,500,501,502};
  int n=0;
  for(int reverse=0;reverse<=1;reverse++)for(int delta:deltas){
   int tick=1000+delta;int[] r=update(p0,p1,1000,tick,duration,accel,vy,reverse!=0);
   out.append("    {\"start_tick\":1000,\"tick\":").append(Integer.toUnsignedLong(tick)).append(",\"reverse\":").append(reverse)
      .append(",\"elapsed_ms\":").append(delta).append(",\"duration_bits\":").append(Integer.toUnsignedLong(Float.floatToRawIntBits(duration)))
      .append(",\"branch\":\"").append(r[0]==0?"active":"completed").append("\",\"point_bits\":[").append(Integer.toUnsignedLong(r[1])).append(',').append(Integer.toUnsignedLong(r[2])).append(',').append(Integer.toUnsignedLong(r[3])).append("]}")
      .append(++n==12?"\n":" ,\n");
  }
  // One rollover sample (small unsigned wrap delta remains a positive signed subtraction).
  int[] rollover=update(p0,p1,0xfffffff0,0x00000018,duration,accel,vy,false);
  out.append("   ,{\"start_tick\":4294967280,\"tick\":24,\"reverse\":0,\"elapsed_ms\":40,\"branch\":\"").append(rollover[0]==0?"active":"completed").append("\",\"point_bits\":[")
     .append(Integer.toUnsignedLong(rollover[1])).append(',').append(Integer.toUnsignedLong(rollover[2])).append(',').append(Integer.toUnsignedLong(rollover[3])).append("]}");
  int[] equal=update(p0,p1,1000,1500,duration,accel,vy,false);
  out.append("\n   ,{\"start_tick\":1000,\"tick\":1500,\"elapsed_ms\":500,\"duration_bits\":").append(Integer.toUnsignedLong(Float.floatToRawIntBits(duration))).append(",\"branch\":\"").append(equal[0]==0?"active":"completed").append("\",\"point_bits\":[")
     .append(Integer.toUnsignedLong(equal[1])).append(',').append(Integer.toUnsignedLong(equal[2])).append(',').append(Integer.toUnsignedLong(equal[3])).append("]}");
  float nextDuration=Float.intBitsToFloat(Float.floatToRawIntBits(duration)+1);
  int[] above=update(p0,p1,1000,1500,nextDuration,accel,vy,false);
  out.append("\n   ,{\"start_tick\":1000,\"tick\":1500,\"elapsed_ms\":500,\"duration_bits\":").append(Integer.toUnsignedLong(Float.floatToRawIntBits(nextDuration))).append(",\"branch\":\"").append(above[0]==0?"active":"completed").append("\",\"point_bits\":[")
     .append(Integer.toUnsignedLong(above[1])).append(',').append(Integer.toUnsignedLong(above[2])).append(',').append(Integer.toUnsignedLong(above[3])).append("]}");
  int[] over=update(p0,p1,1000,1501,nextDuration,accel,vy,false);
  out.append("\n   ,{\"start_tick\":1000,\"tick\":1501,\"elapsed_ms\":501,\"duration_bits\":").append(Integer.toUnsignedLong(Float.floatToRawIntBits(nextDuration))).append(",\"branch\":\"").append(over[0]==0?"active":"completed").append("\",\"point_bits\":[")
     .append(Integer.toUnsignedLong(over[1])).append(',').append(Integer.toUnsignedLong(over[2])).append(',').append(Integer.toUnsignedLong(over[3])).append("]}\n  ]\n}\n");
  File output=new File(args[0]);Files.writeString(output.toPath(),out.toString(),StandardCharsets.UTF_8);
  println("Wrote scalar probe fixture to "+output.getAbsolutePath());
 }

 private int[] setup(float[] endpoints,long tick) throws Exception {
  emu=new EmulatorHelper(currentProgram);
  try {
   init();
   write(TICK,(int)tick);
   for(int j=0;j<6;j++)write(OBJECT+0x1f0+4L*j,Float.floatToRawIntBits(endpoints[j]));
   emu.writeRegister("ESP",STACK);emu.writeMemoryValue(address(STACK),4,RETURN);emu.writeRegister("ECX",OBJECT);emu.writeRegister(emu.getPCRegister(),0x0043b580L);
   stepTo(address(RETURN),"setup");
   return new int[]{read(OBJECT+0x20c),read(OBJECT+0x210),read(OBJECT+0x214),read(OBJECT+0x218),read(OBJECT+0x25c)};
  } finally {emu.dispose();}
 }

 private int[] update(float[] p0,float[] p1,int startTick,int tick,float duration,float accel,float vy,boolean reverse) throws Exception {
  emu=new EmulatorHelper(currentProgram);
  try {
   init();
   write(TICK,tick);write(OBJECT+0x25c,2);write(OBJECT+0x1c4,1);write(OBJECT+0x1c0,(int)ARRAY);write(ARRAY,(int)CHILD);write(CHILD+0xcc,0);
   write(OBJECT+0x208,startTick);write(OBJECT+0x20c,Float.floatToRawIntBits(duration));write(OBJECT+0x210,Float.floatToRawIntBits(accel));write(OBJECT+0x214,Float.floatToRawIntBits(vy));write(OBJECT+0x218,Float.floatToRawIntBits(p0[1]));write(OBJECT+0x220,reverse?1:0);
   for(int i=0;i<3;i++){write(OBJECT+0x1f0+4L*i,Float.floatToRawIntBits(p0[i]));write(OBJECT+0x1fc+4L*i,Float.floatToRawIntBits(p1[i]));}
   emu.writeRegister("ESP",STACK);emu.writeMemoryValue(address(STACK),4,RETURN);emu.writeRegister("ECX",OBJECT);emu.writeRegister(emu.getPCRegister(),0x0043d330L);
   Address stopped=stepUntilAny(address(0x0043d522L),address(0x0043d46dL),"update");
   // Stop before either scalar-result consumer. The completion path selects one stored endpoint;
   // the active path passes the three newly computed float locals.
   long sp=emu.readRegister("ESP").longValue();long values=Integer.toUnsignedLong(read(sp));
   if(stopped.equals(address(0x0043d522L))){
    if(values!=sp+0x14)throw new IllegalStateException("unexpected active scalar pointer 0x"+Long.toHexString(values));
    return new int[]{0,read(values),read(values+4),read(values+8)};
   }
   if(values!=OBJECT+0x1f0&&values!=OBJECT+0x1fc)throw new IllegalStateException("unexpected completed endpoint pointer 0x"+Long.toHexString(values));
   return new int[]{1,read(values),read(values+4),read(values+8)};
  } finally {emu.dispose();}
 }

 private void init() throws Exception {
  for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(n,0);
  Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)emu.writeRegister(cw,0x037fL);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)emu.writeRegister(tag,0xffffL);
  for(Register r:currentProgram.getLanguage().getRegisters())if(r.getName().toUpperCase().matches("ST[0-7]"))emu.writeRegister(r,java.math.BigInteger.ZERO);
 }
 private void stepTo(Address target,String label)throws Exception{stepUntilAny(target,null,label);}
 private Address stepUntilAny(Address first,Address second,String label)throws Exception{
  for(int i=0;i<100000;i++){
   Address pc=emu.getExecutionAddress();if(first.equals(pc)||(second!=null&&second.equals(pc)))return pc;
   if(!emu.step(monitor))throw new IllegalStateException(label+" step stopped at "+emu.getExecutionAddress()+" / "+emu.getLastError());
  }
  throw new IllegalStateException(label+" step limit; pc="+emu.getExecutionAddress());
 }
 private void write(long at,int value)throws Exception{emu.writeMemoryValue(address(at),4,Integer.toUnsignedLong(value));}
 private int read(long at)throws Exception{byte[] b=emu.readMemory(address(at),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private Address address(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private static void vec(StringBuilder out,float[] v,int off){out.append('[').append(v[off]).append(',').append(v[off+1]).append(',').append(v[off+2]).append(']');}
}
