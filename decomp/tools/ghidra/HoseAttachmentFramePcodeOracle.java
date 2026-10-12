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

/** Execute the original Hose Spray attachment owner through its frame commit. */
public class HoseAttachmentFramePcodeOracle extends GhidraScript {
 private static final long PLAYER=0x20010000L,ITEMS=0x20015000L,HOSE=0x20016000L,VALVE=0x20000000L;
 private static final long STACK=0x20fff000L,RETURN=0x70000000L;
 private static final long FRAME_NODES=0x20030000L,OWNER_FRAME=0x20031000L,POSE_HOLDER=0x20032000L,POSE_NODE=0x20033000L,VALVE_VTABLE=0x20034000L;
 private static final long STUB=0x60000000L,GET_FRAME=STUB+0x300L,HUD=0x004571a0L,HIDE=0x004a11a0L;
 private EmulatorHelper emu;
 private final List<String> orderedEvents=new ArrayList<>();

 private static final class Sample {
  final String name;final int[][] axes,position,scale;final int[] parent,getterPosition;final int hoseMode;
  Sample(String n,int[][] a,int[][] p,int[][] s,int[] parents,int[] getter,int mode){name=n;axes=a;position=p;scale=s;parent=parents;getterPosition=getter;hoseMode=mode;}
 }
 private static int f(float value){return Float.floatToRawIntBits(value);}
 private static int[][] axes(float[][][] values){int[][] result=new int[values.length][9];for(int n=0;n<values.length;n++)for(int a=0;a<3;a++)for(int b=0;b<3;b++)result[n][a*3+b]=f(values[n][a][b]);return result;}
 private static int[][] vectors(float[][] values){int[][] result=new int[values.length][3];for(int n=0;n<values.length;n++)for(int a=0;a<3;a++)result[n][a]=f(values[n][a]);return result;}

 @Override public void run() throws Exception {
  String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: HoseAttachmentFramePcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  int[][] identity=axes(new float[][][]{{{1,0,0},{0,1,0},{0,0,1}}});
  int[][] unitScale=vectors(new float[][]{{1,1,1}});
  int[] valve3ModelAxis={0x3f800000,0,0xb3510b46,0,0x3f800000,0,0x33510b46,0x80000000,0x3f800000};
  int[] valve5ModelAxis={0xbf800000,0,0x32d10b46,0,0x3f800000,0,0xb2d10b46,0,0xbf800000};
  Sample[] samples={
   new Sample("owner_attach_root_frame",new int[][]{identity[0]},vectors(new float[][]{{10.25f,-20.5f,30.75f}}),unitScale,new int[]{-1},new int[]{f(2.5f),f(3.25f),f(-4.0f)},2),
   new Sample("owner_attach_parent_rotation",axes(new float[][][]{{{0,-1,0},{1,0,0},{0,0,1}},{{1,0,0},{0,1,0},{0,0,1}}}),vectors(new float[][]{{10,-4,2},{1,2,3}}),vectors(new float[][]{{2,1,0.5f},{1,1,1}}),new int[]{-1,0},new int[]{f(-0.5f),f(8.25f),f(0.125f)},2),
   new Sample("owner_attach_two_parent_chain",axes(new float[][][]{{{0,1,0},{-1,0,0},{0,0,1}},{{1,0.25f,0},{0,1,0.5f},{0,0,1}},{{1,0,0},{0,0.5f,-0.5f},{0,0.5f,0.5f}}}),vectors(new float[][]{{-3,5,8},{2,-1,4},{0.5f,0.25f,-2}}),vectors(new float[][]{{1.5f,0.5f,2},{0.75f,1.25f,0.5f},{1,2,0.25f}}),new int[]{-1,0,1},new int[]{f(4),f(-8),f(16)},2),
   new Sample("owner_attach_signed_zero",axes(new float[][][]{{{-1,0,0},{0,1,0},{0,0,1}}}),vectors(new float[][]{{Float.intBitsToFloat(0x80000000),0,Float.intBitsToFloat(0x80000000)}}),unitScale,new int[]{-1},new int[]{f(-0.0f),f(0.0f),f(1.0f)},2),
   new Sample("training_valve3_frame_from_angle_oracle",new int[][]{identity[0],valve3ModelAxis},vectors(new float[][]{{1368,-176,-2080},{0,0,0}}),vectors(new float[][]{{1,1,1},{1,1,1}}),new int[]{-1,0},new int[]{f(1368),f(-176),f(-2080)},2),
   new Sample("training_valve5_frame_from_angle_oracle",new int[][]{identity[0],valve5ModelAxis},vectors(new float[][]{{416,-176,-2404},{0,0,0}}),vectors(new float[][]{{1,1,1},{1,1,1}}),new int[]{-1,0},new int[]{f(416),f(-176),f(-2404)},2)};
  List<String> rows=new ArrayList<>();for(Sample sample:samples)rows.add(capture(sample));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entries\":[\"0044C840\",\"004977B0\",\"00497300\",\"0042D0A0\",\"00446EE0\",\"0042D2B0\",\"00485A80\",\"00485AA0\",\"00463370\"],\"scope\":\"Original 0044C840 attach owner runs from already-selected Hose Spray through transform, saved valve pointer, mode 3 and first stream-pose-node write. Four frame trees are controlled; the two Training frame trees use basis words captured by the companion TrainingValveFramePcodeOracle (which substitutes Java Math for x87 FCOS/FSIN) and authored getter positions. HUD/hide are boundary stubs. Inventory, actor loading, callback scheduling, and live state are outside this fixture.\",\"samples\":["+String.join(",",rows)+"]}\n");
  println("Captured "+rows.size()+" original Hose attachment frame commits");
 }

 private String capture(Sample c)throws Exception {
  emu=new EmulatorHelper(currentProgram);orderedEvents.clear();
  try {
   emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(ITEMS),new byte[0x100]);emu.writeMemory(a(HOSE),new byte[0xd60]);
   emu.writeMemory(a(VALVE),new byte[0x200]);emu.writeMemory(a(VALVE_VTABLE),new byte[0x100]);emu.writeMemory(a(OWNER_FRAME),new byte[0x80]);
   emu.writeMemory(a(POSE_HOLDER),new byte[0x40]);emu.writeMemory(a(POSE_NODE),new byte[0x40]);emu.writeMemory(a(STACK-0x300),new byte[0x500]);
   for(int n=0;n<c.parent.length;n++)emu.writeMemory(a(FRAME_NODES+0x100L*n),new byte[0x80]);
   zeroCpu();
   w(0x00503a78L,(int)0x20020000L);w(0x20020000L+0x30,(int)PLAYER);
   w(PLAYER+0x3564,(int)ITEMS);w(PLAYER+0x3568,5);w(PLAYER+0x357c,4);w(ITEMS+0x10,(int)HOSE);
   w(HOSE+0xc68,c.hoseMode);w(HOSE+0xcb8,(int)POSE_HOLDER);w(POSE_HOLDER,(int)POSE_NODE);
   w(VALVE,(int)VALVE_VTABLE);w(VALVE_VTABLE+0x18,(int)GET_FRAME);
   w(VALVE+0xd0,(int)(FRAME_NODES+0x100L*(c.parent.length-1)));
   for(int n=0;n<c.parent.length;n++){
    long node=FRAME_NODES+0x100L*n;w(node,0x004dfc30);
    for(int i=0;i<9;i++)w(node+4+4L*i,c.axes[n][i]);
    for(int i=0;i<3;i++){w(node+0x28+4L*i,c.position[n][i]);w(node+0x34+4L*i,c.scale[n][i]);}
    w(node+0x40,c.parent[n]<0?0:(int)(FRAME_NODES+0x100L*c.parent[n]));
   }
   for(int i=0;i<3;i++)w(OWNER_FRAME+0x28+4L*i,c.getterPosition[i]);
   w(STACK,(int)RETURN);w(STACK+4,(int)VALVE);w(STACK+8,1001);
   emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",PLAYER);emu.getEmulator().setExecuteAddress(0x0044c840L);
   boolean returned=false;
   for(int steps=0;steps<500000;steps++){
    long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN){returned=true;break;}
    if(pc==GET_FRAME){ret(0,(int)OWNER_FRAME);continue;}
    if(pc==HUD){long sp=emu.readRegister("ESP").longValue();long receiver=emu.readRegister("ECX").longValue();String event="hud:"+(receiver-PLAYER)+":"+r(sp+4)+":"+r(sp+8);orderedEvents.add(event);ret(8,0);continue;}
    if(pc==HIDE){long sp=emu.readRegister("ESP").longValue();long receiver=emu.readRegister("ECX").longValue();String event="hide:"+(receiver-PLAYER)+":"+r(sp+4);orderedEvents.add(event);ret(4,0);continue;}
    if(!emu.step(monitor))throw new IllegalStateException("Attachment P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   }
   if(!returned)throw new IllegalStateException("Attachment owner did not return; stopped at "+emu.getExecutionAddress());
   return "{\"name\":\""+c.name+"\",\"valve_root_id\":301,\"valve_frame_root\":"+(c.parent.length-1)+",\"valve_frame_nodes\":["+nodeJson(c)+"],\"valve_getter_position_bits\":["+unsigned(c.getterPosition)+"],\"first_stream_pose_node_id\":701,\"hose_mode_before\":"+c.hoseMode+",\"hose_mode_after\":"+r(HOSE+0xc68)+",\"hose_valve_root_id\":"+(r(HOSE+0xc6c)==(int)VALVE?301:-1)+",\"hose_frame_position_bits\":["+readWords(HOSE+0xc70,3)+"],\"hose_valve_offset_bits\":["+readWords(HOSE+0xc7c,3)+"],\"hose_current_stream_pose_node_id\":"+(r(HOSE+0xcd0)==(int)POSE_NODE?701:-1)+",\"stream_pose_node_position_bits\":["+readWords(POSE_NODE+8,3)+"],\"ordered_boundary_events\":["+quoted(orderedEvents)+"]}";
  }finally{emu.dispose();}
 }

 private String nodeJson(Sample c){List<String> rows=new ArrayList<>();for(int n=0;n<c.parent.length;n++)rows.add("{\"parent\":"+c.parent[n]+",\"axes_bits\":["+unsigned(c.axes[n])+"],\"position_bits\":["+unsigned(c.position[n])+"],\"scale_bits\":["+unsigned(c.scale[n])+"]}");return String.join(",",rows);}
 private String unsigned(int[] values){List<String> rows=new ArrayList<>();for(int value:values)rows.add(Integer.toUnsignedString(value));return String.join(",",rows);}
 private String readWords(long address,int count)throws Exception{List<String> rows=new ArrayList<>();for(int i=0;i<count;i++)rows.add(Integer.toUnsignedString(r(address+4L*i)));return String.join(",",rows);}
 private String quoted(List<String> values){List<String> rows=new ArrayList<>();for(String value:values)rows.add("\""+value+"\"");return String.join(",",rows);}
 private void zeroCpu()throws Exception{
  for(String name:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(name,0);
  emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  Register control=currentProgram.getLanguage().getRegister("FPUControlWord");if(control!=null)emu.writeRegister(control,0x037fL);
  Register status=currentProgram.getLanguage().getRegister("FPUStatusWord");if(status!=null)emu.writeRegister(status,0);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)emu.writeRegister(tag,0xffffL);
  for(Register reg:currentProgram.getLanguage().getRegisters())if(reg.getName().toUpperCase().matches("ST[0-7]"))emu.writeRegister(reg,BigInteger.ZERO);
 }
 private void ret(int argumentBytes,int eax)throws Exception{long sp=emu.readRegister("ESP").longValue();long target=Integer.toUnsignedLong(r(sp));emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+argumentBytes);emu.getEmulator().setExecuteAddress(target);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[] b=emu.readMemory(a(p),4);return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
}
