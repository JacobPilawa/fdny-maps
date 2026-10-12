import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute original 0047F960 with explicit fake renderer state and capture the import. */
public class AshParticlePolyPcodeOracle extends GhidraScript {
 private static final long PARTICLE=0x21000000L,CONTAINER=0x21001000L,LIST=0x21001100L;
 private static final long RECORDS=0x21001200L,CONTEXT=0x21002000L,TABLE=0x21002100L;
 private static final long RESOURCE=0x21002200L,STACK=0x21003000L,IMPORT_STUB=0x21004000L;
 private static final long RETURN=0x70000000L,POOL=0x0047f850L,DRAW=0x0047f960L;
 private static final long ADD_POLY_IAT=0x004dc0d0L,BOUNDS=0x0048d490L;
 private final List<int[]> submissions=new ArrayList<>();
 private final List<int[]> submitArgs=new ArrayList<>();
 private static final int[][] FRAME_WORDS={
  {0x3f000000,0x3f800000,0x3e4ccccd,0x3f666666,0x3dcccccd,0},
  {0x3f000000,0x3f800000,0x3e4ccccd,0x3f666666,0x3f000000,0},
  {0x3f000000,0x3f800000,0x3e4ccccd,0x3f000000,0x3f000000,0x3f000000},
  {0x3f800000,0x3f800000,0x3e4ccccd,0,0,0},
  {0x3f800000,0,0x3e4ccccd,0,0,0}
 };
 private static final int[] POS_OFFSETS={0xd8,0xfc,0x120,0x144};
 private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
 private void w(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private void b(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),1,Integer.toUnsignedLong(v));}
 private int r(EmulatorHelper e,long p)throws Exception{byte[]b=e.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private long u(EmulatorHelper e,String reg)throws Exception{return Integer.toUnsignedLong((int)e.readRegister(reg).longValue());}
 private void resetCpu(EmulatorHelper e)throws Exception{
  for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})e.writeRegister(n,0);
  e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);
  e.writeRegister("FS_OFFSET",0);w(e,0,0);
  Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)e.writeRegister(cw,0x037fL);
  Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)e.writeRegister(sw,0);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)e.writeRegister(tag,0xffffL);
  for(Register reg:currentProgram.getLanguage().getRegisters())if(reg.getName().toUpperCase().matches("ST[0-7]"))e.writeRegister(reg,java.math.BigInteger.ZERO);
 }
 private void prepare(EmulatorHelper e,int resourceId)throws Exception{
  e.writeMemory(a(PARTICLE),new byte[0x200]);e.writeMemory(a(CONTAINER),new byte[0x100]);
  e.writeMemory(a(LIST),new byte[0x100]);e.writeMemory(a(RECORDS),new byte[0x200]);
  e.writeMemory(a(CONTEXT),new byte[0x200]);e.writeMemory(a(TABLE),new byte[0x40]);
  e.writeMemory(a(RESOURCE),new byte[0x40]);e.writeMemory(a(STACK),new byte[0x400]);
  w(e,CONTAINER,(int)LIST);w(e,CONTAINER+4,5);w(e,CONTAINER+0x18,0x3c);
  for(int i=0;i<5;i++){
   long rec=RECORDS+0x20L*i;w(e,LIST+4L*i,(int)rec);w(e,rec,resourceId);
   for(int j=0;j<6;j++)w(e,rec+4L*(j+1),FRAME_WORDS[i][j]);
  }
  // Renderer table is the explicit +8 subcontext selected by context byte +0x160=0.
  w(e,CONTEXT+0x160,0);w(e,CONTEXT+0x50,(int)TABLE);w(e,CONTEXT+0x54,1);
  w(e,CONTEXT+0x80,0x21002300);w(e,TABLE,(int)RESOURCE);w(e,RESOURCE,0x21002400);w(e,RESOURCE+4,resourceId);
  w(e,0x005030bc,1); // Original lazy-geometry template initializer already completed.
  w(e,ADD_POLY_IAT,(int)IMPORT_STUB);
  w(e,STACK,(int)RETURN);
 }
 private int runEntry(EmulatorHelper e,long entry,long self,int[]args)throws Exception{
  resetCpu(e);w(e,STACK,(int)RETURN);for(int i=0;i<args.length;i++)w(e,STACK+4L*(i+1),args[i]);
  e.writeRegister("ESP",STACK);e.writeRegister("ECX",self);e.getEmulator().setExecuteAddress(entry);
  int captured=0;
  for(int n=0;n<300000;n++){
   long pc=e.getExecutionAddress().getOffset();if(pc==RETURN)return captured;
   if(pc==BOUNDS){long sp=u(e,"ESP"),ret=Integer.toUnsignedLong(r(e,sp));e.writeRegister("ESP",sp+8);e.writeRegister("EAX",1);e.getEmulator().setExecuteAddress(ret);continue;}
   if(pc==IMPORT_STUB){
    long sp=u(e,"ESP"),ret=Integer.toUnsignedLong(r(e,sp));
    long vertices=Integer.toUnsignedLong(r(e,sp+8));
    if(vertices!=PARTICLE+0xd8)throw new IllegalStateException("AddPolyOnce vertex pointer unexpected: 0x"+Long.toHexString(vertices));
    // Capture exactly the 4 x 9 dwords copied by the Genesis.dll wrapper.
    int[] words=new int[36];for(int i=0;i<words.length;i++)words[i]=r(e,vertices+4L*i);
    submissions.add(words);
    int[] argsAtImport=new int[7];for(int i=0;i<argsAtImport.length;i++)argsAtImport[i]=r(e,sp+4L*(i+1));submitArgs.add(argsAtImport);
    captured++;
    e.writeRegister("ESP",sp+4);e.writeRegister("EAX",0);e.getEmulator().setExecuteAddress(ret);continue;
   }
   if(!e.step(monitor))throw new IllegalStateException("0047F960 P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
  }
  throw new IllegalStateException("0047F960 instruction limit at "+e.getExecutionAddress()+" (submissions="+captured+")");
 }
 private String sample(String name,int state,int current,int next,int elapsed,int resourceId,int[]spawn,int[]positions)throws Exception{
  EmulatorHelper e=new EmulatorHelper(currentProgram);
  try{
   prepare(e,resourceId);
   runEntry(e,POOL,PARTICLE,new int[]{(int)CONTAINER,0});
   w(e,PARTICLE+4,current);w(e,PARTICLE+8,next);w(e,PARTICLE+0x14,1000);
   w(e,PARTICLE+0x18,0x3c);w(e,PARTICLE+0x1c,0x3f800000);w(e,PARTICLE+0x20,0x3f800000);
   b(e,PARTICLE+0x30,1);w(e,PARTICLE+0xa8,0x3f800000);
   w(e,PARTICLE+0xb0,state);b(e,PARTICLE+0xa0,0);b(e,PARTICLE+0xac,0);b(e,PARTICLE+0xad,0);
   w(e,PARTICLE+0xa4,0x21002500);w(e,PARTICLE+0xcc,0);w(e,PARTICLE+0xd4,0);
   for(int j=0;j<3;j++)w(e,PARTICLE+0x5c+4L*j,spawn[j]);
   for(int i=0;i<4;i++)for(int j=0;j<3;j++)w(e,PARTICLE+POS_OFFSETS[i]+4L*j,positions[i*3+j]);
   if(r(e,PARTICLE+0xb0)!=state||r(e,PARTICLE+0xd8)!=positions[0]||((r(e,PARTICLE+0x78)&255)!=1))throw new IllegalStateException("Controlled pre-draw state was not seeded");
   int now=1000+elapsed;w(e,0x00502afcL,now);
   submissions.clear();submitArgs.clear();int submitCount=runEntry(e,DRAW,PARTICLE,new int[]{(int)CONTEXT});
   if(submitCount!=1||submissions.size()!=1)throw new IllegalStateException(name+" expected one intercepted AddPolyOnce call, observed "+submitCount);
   int[] packet=submissions.get(0);
   int[] argsAtImport=submitArgs.get(0);
   StringBuilder out=new StringBuilder("{\"name\":\"").append(name).append("\",\"state\":").append(state).append(",\"current\":").append(current)
    .append(",\"next\":").append(next).append(",\"now\":").append(Integer.toUnsignedString(now))
   .append(",\"timestamp\":1000,\"flags\":60,\"spawn_position_bits\":[").append(Integer.toUnsignedString(spawn[0])).append(',').append(Integer.toUnsignedString(spawn[1])).append(',').append(Integer.toUnsignedString(spawn[2])).append("],\"positions_bits\":[");
   for(int i=0;i<12;i++){if(i>0)out.append(',');out.append(Integer.toUnsignedString(positions[i]));}
   out.append("],\"vertices_bits\":[");for(int i=0;i<packet.length;i++){if(i>0)out.append(',');out.append(Integer.toUnsignedString(packet[i]));}
   out.append("],\"submit_count\":").append(submitCount).append(",\"submit_args\":[").append(words(argsAtImport)).append("],\"world\":\"").append(Long.toHexString(r(e,CONTEXT+0x80))).append("\",\"resource\":\"").append(Long.toHexString(r(e,RESOURCE))).append("\",\"resource_id\":").append(r(e,RESOURCE+4)).append(",\"descriptor_id\":").append(r(e,RECORDS+0x20L*current)).append("}");
   return out.toString();
  }finally{e.dispose();}
 }
 private String words(int[] values){List<String>x=new ArrayList<>();for(int v:values)x.add(Integer.toUnsignedString(v));return String.join(",",x);}
 @Override public void run()throws Exception{
  String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshParticlePolyPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  int[][] positions={
   {0x3f800000,0x40000000,0xc0400000,0x40800000,0x40900000,0xc0a00000,0x40c00000,0x40e00000,0xc1100000,0x41100000,0x41200000,0xc1200000},
   {0xc1200000,0x3f000000,0x41200000,0xc1100000,0x3fc00000,0x41100000,0xc1000000,0x40100000,0x41000000,0xc0e00000,0x40400000,0x40e00000},
   {0x3eaaaaab,0xc1200000,0x42a00000,0x3f000000,0xc1100000,0x42900000,0x3f800000,0xc1000000,0x42800000,0x3fc00000,0xc0e00000,0x42700000}
  };
  String[] names={"training_initial_state0_zero_buffer","controlled_state0_nonzero_spawn","controlled_state1_frame0_quarter_id200","controlled_state1_frame1_half_id200","controlled_state1_frame1_half_id201","controlled_state1_frame2_three_quarter_id200"};
  int[] states={0,0,1,1,1,1},current={0,0,0,1,1,2},next={1,1,1,2,2,3},elapsed={0,0,125,250,250,375},ids={200,200,200,200,201,200};
  int[][] allPositions={new int[12],new int[12],positions[0],positions[1],positions[1],positions[2]};
  int[][] spawns={{0,0,0},{0x41200000,0xc1200000,0x42a00000},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};List<String> rows=new ArrayList<>();
  for(int i=0;i<names.length;i++)rows.add(sample(names[i],states[i],current[i],next[i],elapsed[i],ids[i],spawns[i],allPositions[i]));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code; controlled context/resource table; IAT trampoline captures AddPolyOnce vertex payload and arguments\",\"scope\":\"Original 0047F850 constructor and 0047F960 callback; state 0 cases include constructor-default zero spawn and a controlled nonzero spawn, with other fake object storage zeroed; states 1 are controlled four-vertex variants; culling disabled; byte +0xAD animation deadline update disabled; fake context selects +8 resource table; 0048D490 valid-index bounds service returns true; imported geWorld_AddPolyOnce is intercepted and the 36-word source buffer plus seven cdecl arguments are captured\",\"uv_bits\":[0,0,1065185444,0,1065185444,1065185444,0,1065185444],\"samples\":["+String.join(",",rows)+"]}\n");
  println("Captured "+rows.size()+" original 0047F960 ash polygon payloads");
 }
}
