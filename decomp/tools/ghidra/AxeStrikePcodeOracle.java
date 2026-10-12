import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Runs original 00460B90; pose, collision, engine effects, and hit leaf are explicit boundaries. */
public class AxeStrikePcodeOracle extends GhidraScript {
 private static final long ENTRY=0x00460b90L, AXE=0x20010000L, PLAYER=0x20020000L, STACK=0x20fff000L, RETURN=0x70000000L;
 private static final long CANDIDATE=0x20030000L,NODE=0x20031000L,WORLD=0x20040000L,CHILD=0x20050000L,HOOK=0x20060000L,STRING=0x00501908L;
 private static final long FEC0=0x0045fec0L, VEC=0x00446390L, ADD=0x0042d050L, FRAMEVEC=0x00447830L, VECINPLACE=0x00446ee0L, VECAXIS=0x00447710L, FRAME=0x0049e770L, CROSS=0x0043b2d0L, COLLISION=0x00492ce0L;
 private static final long STR=0x00402790L,STAT=0x00486980L,FRAMECOPY=0x00447890L,PARENT=0x00483a20L,SOUND=0x0045fb90L,FRAMEINIT=0x0042d2d0L,NODEGET=0x00444c00L,NODEUPDATE=0x00445c40L,STOP=0x0049f340L,ITER=0x00481c00L,BOX=0x004816c0L,HIT=0x0043e2a0L,HIDE=0x004a11a0L,DELETE=0x00429fe0L;
 private EmulatorHelper emu; private final List<String> events=new ArrayList<>(); private final List<String> trace=new ArrayList<>(); private String mode; private long candidateResult; private boolean collisionHit,boxHit; private long collisionCalls; private int targetType=4;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private long r64(long p)throws Exception{byte[]b=emu.readMemory(a(p),8);long v=0;for(int i=0;i<8;i++)v|=(long)(b[i]&255)<<(8*i);return v;}
 private String vec(long p)throws Exception{return "["+Float.toString(Float.intBitsToFloat(r(p)))+","+Float.toString(Float.intBitsToFloat(r(p+4)))+","+Float.toString(Float.intBitsToFloat(r(p+8)))+"]";}
 private String vecBits(long p)throws Exception{return "{\"values\":"+vec(p)+",\"bits\":["+Integer.toUnsignedString(r(p))+","+Integer.toUnsignedString(r(p+4))+","+Integer.toUnsignedString(r(p+8))+"]}";}
 private String deltaBits(long start,long end)throws Exception{int[]bits=new int[3];for(int i=0;i<3;i++)bits[i]=Float.floatToRawIntBits(Float.intBitsToFloat(r(end+4L*i))-Float.intBitsToFloat(r(start+4L*i)));return "{\"values\":["+Float.intBitsToFloat(bits[0])+","+Float.intBitsToFloat(bits[1])+","+Float.intBitsToFloat(bits[2])+"],\"bits\":["+Integer.toUnsignedString(bits[0])+","+Integer.toUnsignedString(bits[1])+","+Integer.toUnsignedString(bits[2])+"]}";}
 private void wf(long p,float v)throws Exception{w(p,Float.floatToRawIntBits(v));}
 private long arg(int i)throws Exception{return Integer.toUnsignedLong(r(emu.readRegister("ESP").longValue()+4+4L*i));}
 private void event(String label,long... values){List<String>x=new ArrayList<>();for(long v:values)x.add(Long.toUnsignedString(v));events.add("{\"name\":\""+label+"\",\"args\":["+String.join(",",x)+"]}");}
 private void ret(int stackBytes,long eax)throws Exception{long sp=emu.readRegister("ESP").longValue();emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+stackBytes);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(r(sp)));}
 private void setVec(long p,float x,float y,float z)throws Exception{wf(p,x);wf(p+4,y);wf(p+8,z);}
 private void stubVector(long pc)throws Exception{long sp=emu.readRegister("ESP").longValue(),out=0;int clean=0;if(pc==FRAMEVEC){out=arg(0);clean=4;}else if(pc==VEC||pc==ADD||pc==VECAXIS||pc==CROSS){out=arg(0);clean=8;}else if(pc==FRAME){out=arg(0);clean=4;}else if(pc==VECINPLACE){out=arg(0);clean=4;}setVec(out,0,0,0);event("pose:"+Long.toHexString(pc),emu.readRegister("ECX").longValue(),out);ret(clean,out);}
 private void init(String scenario)throws Exception{mode=scenario;events.clear();trace.clear();emu=new EmulatorHelper(currentProgram);emu.writeMemory(a(AXE),new byte[0x600]);emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(CANDIDATE),new byte[0x300]);emu.writeMemory(a(NODE),new byte[0x20]);emu.writeMemory(a(WORLD),new byte[0x100]);emu.writeMemory(a(CHILD),new byte[0x100]);emu.writeMemory(a(0x20061000L),new byte[0x40]);emu.writeMemory(a(STACK-0x200),new byte[0x600]);for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);w(AXE+0x54,(int)PLAYER);w(AXE+0x1fc,0);wf(AXE+0x340,0);wf(AXE+0x33c,2);w(PLAYER,(int)0x20061000);w(0x20061018L,(int)HOOK);w(CHILD,(int)0x20061000);w(0x2006100cL,(int)HOOK);w(PLAYER+0x34a4,(int)CHILD);wf(PLAYER+0x34a8,0);wf(PLAYER+0x34ac,1);wf(PLAYER+0x34b0,0);wf(PLAYER+0x34b4,1);wf(PLAYER+0x34b8,0);wf(PLAYER+0x34bc,0);wf(PLAYER+0x34c0,0);wf(PLAYER+0x34c4,0);wf(PLAYER+0x34c8,1);wf(PLAYER+0x34cc,0);wf(PLAYER+0x34d0,0);wf(PLAYER+0x34d4,0);w(0x00502aacL,(int)WORLD);w(0x00503a7cL,(int)WORLD);w(0x00502e04L,0);w(0x00502afcL,0x12345678);w(0x00501908L,2);w(0x00501918L,1);
  if(scenario.equals("collision_success_empty")){setVec(PLAYER+0x34a8,0.1234567f,-0.33333334f,1.001f);setVec(PLAYER+0x34b4,1.234567f,-2.7182817f,0.0001234567f);setVec(PLAYER+0x34c0,-0.7654321f,0.31415927f,3.1415927f);setVec(PLAYER+0x34cc,12.34567f,-0.0001234567f,-98.76543f);w(AXE+0x1fc,1);}
  if(scenario.equals("candidate_hit_flags_1")){setVec(PLAYER+0x34a8,-0.27182817f,0.5772157f,-1.4142135f);setVec(PLAYER+0x34b4,0.9876543f,-1.1111112f,0.0009876543f);setVec(PLAYER+0x34c0,2.3025851f,-0.6931472f,0.70710677f);setVec(PLAYER+0x34cc,-3.3333333f,0.00031415927f,42.42424f);w(AXE+0x1fc,0);}
  collisionCalls=0;collisionHit=scenario.equals("collision_success_empty")||scenario.equals("candidate_hit_flags_1");boxHit=scenario.equals("candidate_hit_flags_1");candidateResult=scenario.equals("candidate_hit_flags_1")?1:0;
  if(scenario.equals("strict_time_reject")){wf(AXE+0x340,0.275f);wf(AXE+0x33c,0);}else{wf(AXE+0x340,0);wf(AXE+0x33c,1);}
  if(scenario.equals("candidate_hit_flags_1")){w(CANDIDATE+0x18,targetType);w(NODE,(int)CANDIDATE);w(NODE+4,0);}
  w(STACK,(int)RETURN);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",AXE);emu.getEmulator().setExecuteAddress(ENTRY);
 }
 private String runCase(String scenario)throws Exception{init(scenario);int poseCalls=0;String exit="return";for(int step=0;step<12000;step++){long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)break;if(pc==0x004d1521L){event("emulator-seh-return-boundary:004D1521",emu.readRegister("ESP").longValue());exit="seh-boundary";break;}if(pc<0x100){event("emulator-invalid-return-boundary",pc,emu.readRegister("ESP").longValue());exit="invalid-return-boundary";break;}if(trace.size()<1200)trace.add(Long.toHexString(pc));
   if(pc==FEC0){event("gate:0045FEC0",emu.readRegister("ECX").longValue(),arg(0));ret(4,0);continue;}
   if(pc==FRAMEVEC||pc==VEC||pc==ADD||pc==VECINPLACE||pc==VECAXIS||pc==FRAME||pc==CROSS){event("pose-helper:"+Long.toHexString(pc),emu.readRegister("ECX").longValue(),arg(0),arg(1));poseCalls++;}
   if(pc==COLLISION){long[]v=new long[7];v[0]=emu.readRegister("ECX").longValue();for(int i=0;i<6;i++)v[i+1]=arg(i);events.add("{\"name\":\"collision:00492CE0\",\"args\":["+String.join(",",java.util.Arrays.stream(v).mapToObj(Long::toUnsignedString).toArray(String[]::new))+"],\"start\":"+vecBits(v[1])+",\"mins\":"+vecBits(v[2])+",\"maxs\":"+vecBits(v[3])+",\"displacement\":"+vecBits(v[4])+"}");collisionCalls++;ret(24,collisionHit?1:0);continue;}
   if(pc==STR){event("string:00402790",emu.readRegister("ECX").longValue(),arg(0));w(emu.readRegister("ECX").longValue(),(int)STRING);ret(4,emu.readRegister("ECX").longValue());continue;}
   if(pc==STAT){event("stat:00486980",emu.readRegister("ECX").longValue(),arg(0),arg(1));ret(8,0);continue;}
   if(pc==FRAMECOPY){event("frame-copy:00447890",emu.readRegister("ECX").longValue(),arg(0));ret(4,0);continue;}
   if(pc==PARENT){event("parent:00483A20",emu.readRegister("ECX").longValue(),arg(0),arg(1),arg(2));ret(8,0);continue;}
   if(pc==SOUND){event("sound:0045FB90",emu.readRegister("ECX").longValue(),arg(0));ret(4,0);continue;}
   if(pc==FRAMEINIT){event("frame-init:0042D2D0",emu.readRegister("ECX").longValue(),arg(0));ret(0,0);continue;}
   if(pc==NODEGET){event("node-get:00444C00",emu.readRegister("ECX").longValue(),arg(0));ret(4,CHILD);continue;}
   if(pc==NODEUPDATE){event("node-update:00445C40",emu.readRegister("ECX").longValue());ret(0,0);continue;}
   if(pc==STOP){event("stop-sound:0049F340",emu.readRegister("ECX").longValue());ret(0,0);continue;}
   if(pc==ITER){long out=emu.readRegister("ECX").longValue();event("iterator:00481C00",out);w(out,scenario.equals("candidate_hit_flags_1")?1:0);w(out+4,scenario.equals("candidate_hit_flags_1")?(int)NODE:0);ret(0,out);continue;}
   if(pc==BOX){event("box:004816C0",emu.readRegister("ECX").longValue(),arg(0),arg(1),arg(2));ret(8,boxHit?1:0);continue;}
   if(pc==HIT){event("hit-leaf:0043E2A0",emu.readRegister("ECX").longValue());ret(0,candidateResult);continue;}
   if(pc==HIDE){event("hide:004A11A0",emu.readRegister("ECX").longValue(),arg(0));ret(4,0);continue;}
   if(pc==DELETE){event("iterator-delete:00429FE0",emu.readRegister("ECX").longValue(),arg(0));ret(4,0);continue;}
   if(pc==HOOK){event("player-virtual-18",emu.readRegister("ECX").longValue());ret(0,CHILD);continue;}
   if(!emu.step(monitor))throw new IllegalStateException("P-code failure at "+emu.getExecutionAddress()+" for "+scenario+": "+emu.getLastError()+"; trace="+trace);
  }
  if(emu.getExecutionAddress().getOffset()!=RETURN&&!exit.equals("seh-boundary")&&!exit.equals("invalid-return-boundary"))throw new IllegalStateException("instruction limit at "+emu.getExecutionAddress()+" scenario="+scenario+" trace="+trace);
  long base=PLAYER+0x34a4;String frame="{\"base\":"+base+",\"plus4\":"+vecBits(base+4)+",\"plus10\":"+vecBits(base+0x10)+",\"plus1c\":"+vecBits(base+0x1c)+",\"player_plus34cc\":"+vecBits(PLAYER+0x34cc)+",\"axe_signed_side_byte\":"+(byte)r(AXE+0x1fc)+"}";
  long[]writeAddresses={AXE+0x368,AXE+0x36c,AXE+0x340,AXE+0x33c,AXE+0x184,AXE+0x188,AXE+0x18c,PLAYER+0x34a4,PLAYER+0x34cc,PLAYER+0x34d0,PLAYER+0x34d4,0x00502e04L};StringBuilder writes=new StringBuilder("[");for(int i=0;i<writeAddresses.length;i++){if(i>0)writes.append(',');long address=writeAddresses[i];writes.append("{\"address\":").append(Long.toUnsignedString(address)).append(",\"value\":").append(Integer.toUnsignedString(r(address))).append('}');}writes.append(']');
  return "{\"name\":\""+scenario+"\",\"exit_boundary\":\""+exit+"\",\"frame_input\":"+frame+",\"global_threshold_004E28B0_bits\":\"0x"+Long.toHexString(r64(0x004e28b0L))+"\",\"pose_helpers_executed\":"+poseCalls+",\"collision_calls\":"+collisionCalls+",\"collision_result_hit\":"+(collisionHit?"true":"false")+",\"writes\":"+writes+",\"events\":["+String.join(",",events)+"]}";
 }
 @Override public void run()throws Exception{String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AxeStrikePcodeOracle.java output.json original.exe");StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");List<String>rows=new ArrayList<>();for(String s:new String[]{"strict_time_reject","collision_miss","collision_success_empty","candidate_hit_flags_1"})rows.add(runCase(s));Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00460B90\",\"scope\":\"Actual 00460B90 body executes with original pose helpers 00447830, 00446EE0, 0049E770, 00446390, 00447710, 0042D050, and 0043B2D0. Gate, collision, engine effects, candidate box test, hit leaf, and iterator setup are controlled boundaries. collision_success_empty stops at SEH handler 004D1521; candidate_hit_flags_1 reaches the final return but the synthetic stack yields invalid PC 2. Each sample records its exit boundary. No live Training parity claimed.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured original Axe strike handler cases");}
}
