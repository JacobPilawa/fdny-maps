import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute the original Player hose attach/detach inventory prefix. */
public class HoseItemSelectionPcodeOracle extends GhidraScript {
 private static final long PLAYER=0x20010000L,ITEMS=0x20015000L,HOSE=0x20016000L,VALVE=0x20000000L;
 private static final long STACK=0x20fff000L,RETURN=0x70000000L,FRAME=0x20017000L;
 private static final long STUB=0x60000000L,HUD=0x004571a0L,HIDE=0x004a11a0L,MODE=0x00463370L,TRANSFORM=0x004977b0L;
 private static final long AXE_GATE=0x0045f930L,AXE_CHILD_GATE=0x0045fa50L;
 private static final long AXE_PRIMARY_VTABLE=0x2001e000L,AXE_SECONDARY_VTABLE=0x2001e100L;
 private EmulatorHelper emu;
 private final List<String> itemEvents=new ArrayList<>(),uiEvents=new ArrayList<>(),orderedEvents=new ArrayList<>();

 private static final class Sample {
  final String name;final int action,selected,mode;final int[] present;final boolean[] canDeactivate;
  final int axePrimaryEnabled,axePrimaryState,axeSecondaryState;
  Sample(String n,int action,int selected,int mode,int[] present,boolean[] can,int primaryEnabled,int primaryState,int secondaryState){this.name=n;this.action=action;this.selected=selected;this.mode=mode;this.present=present;this.canDeactivate=can;this.axePrimaryEnabled=primaryEnabled;this.axePrimaryState=primaryState;this.axeSecondaryState=secondaryState;}
 }

 @Override public void run() throws Exception {
  String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: HoseItemSelectionPcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  int[] standard={-1,1,-1,-1,4};boolean[] can={true,true,true,true,true};
  List<Sample> cases=List.of(
   new Sample("attach_from_slot0",1001,0,2,standard,can,0,0,0),
   new Sample("attach_blocked_current_item",1001,1,2,standard,new boolean[]{true,false,true,true,true},0,0,3),
   new Sample("attach_switch_current_item",1001,1,2,standard,can,0,0,0),
   new Sample("axe_idle_gate",1001,1,2,standard,can,1,0,0),
   new Sample("axe_secondary_busy_gate",1001,1,2,standard,can,0,0,3),
   new Sample("axe_primary_disabled_busy_gate",1001,1,2,standard,can,0,3,0),
   new Sample("axe_enabled_primary_busy_gate",1001,1,2,standard,can,1,3,0),
   new Sample("attach_already_selected",1001,4,2,standard,can,0,0,0),
   new Sample("attach_hose_missing",1001,0,2,new int[]{0,-1,-1,-1,-1},can,0,0,0),
   new Sample("detach_first_slot",1002,4,3,standard,new boolean[]{false,true,true,true,true},0,0,0),
   new Sample("detach_first_candidate_is_selected",1002,1,3,new int[]{-1,1,-1,-1,4},can,0,0,0),
   new Sample("detach_mode_already2",1002,4,2,standard,can,0,0,0));
  List<String> rows=new ArrayList<>();for(Sample c:cases)rows.add(capture(c));String emptyReselect=captureEmptyReselect();
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entries\":[\"0044C840\",\"0044C360\",\"0044C2A0\",\"00463370\",\"0045F930\",\"0045FA50\"],\"scope\":\"Owner inventory-selection transaction. Real owner, array helpers, mode setter, scan order, Training Axe root +0x38 gate 0045F930 and both child +0x38 leaf gates 0045FA50 execute as original P-code. The Axe primary child call is conditional on Axe+0x1FC; the secondary child runs only if the primary is disabled or accepts. The child leaves read Axe+0x568 and Axe+0x9B0 through child+0x368. Other item +0x38/+0x3C, HUD 004571A0 and hide 004A11A0 remain controlled service boundaries. Attach samples stop at 004977B0 before valve/frame computation. No live inventory capture.\",\"samples\":["+String.join(",",rows)+"],\"helper_samples\":["+emptyReselect+"]}\n");
  println("Captured "+rows.size()+" original hose item-selection transactions");
 }

 private String capture(Sample c)throws Exception {
  emu=new EmulatorHelper(currentProgram);itemEvents.clear();uiEvents.clear();orderedEvents.clear();
  try {
   emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(ITEMS),new byte[0x100]);emu.writeMemory(a(HOSE),new byte[0xd60]);
   emu.writeMemory(a(VALVE),new byte[0x200]);emu.writeMemory(a(FRAME),new byte[0x100]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
   for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
   emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
   w(0x00503a78L,(int)0x20020000L);w(0x20020000L+0x30,(int)PLAYER);
   w(PLAYER+0x3564,(int)ITEMS);w(PLAYER+0x3568,5);w(PLAYER+0x357c,c.selected);
   w(VALVE+0xd0,(int)FRAME);w(HOSE+0xc68,c.mode);
   int[] vtables=new int[5];long[] objects=new long[5];
   for(int i=0;i<5;i++){
    if(c.present[i]<0)continue;
    objects[i]=i==4?HOSE:0x20018000L+i*0x1000L;vtables[i]=(int)(0x2001d000L+i*0x100L);
    emu.writeMemory(a(objects[i]),new byte[i==4?0xd60:(i==1?0xa94:0x100)]);w(objects[i],vtables[i]);
    w(ITEMS+i*4,(int)objects[i]);
    w(Integer.toUnsignedLong(vtables[i])+0x38,(int)(STUB+0x100+i*0x20));
    w(Integer.toUnsignedLong(vtables[i])+0x3c,(int)(STUB+0x108+i*0x20));
   }
   int axeRootCalls=0,axePrimaryCalls=0,axeSecondaryCalls=0;long axeGateReturn=-1;
   if(objects[1]!=0){
    long axe=objects[1];w(Integer.toUnsignedLong(vtables[1])+0x38,(int)AXE_GATE);
    w(axe+0x1fc,c.axePrimaryEnabled);w(axe+0x568,c.axePrimaryState);w(axe+0x9b0,c.axeSecondaryState);
    w(axe+0x200,(int)AXE_PRIMARY_VTABLE);w(axe+0x648,(int)AXE_SECONDARY_VTABLE);
    w(AXE_PRIMARY_VTABLE+0x38,(int)AXE_CHILD_GATE);w(AXE_SECONDARY_VTABLE+0x38,(int)AXE_CHILD_GATE);
   }
   // Restore the Hose Spray mode after its backing item was initialized.
   if(objects[4]!=0)w(HOSE+0xc68,c.mode);
   for(int i=0;i<5;i++)if(objects[i]!=0)w(STUB+0x100+i*0x20,c.canDeactivate[i]?1:0);
   w(STACK,(int)RETURN);w(STACK+4,(int)VALVE);w(STACK+8,c.action);
   emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",PLAYER);emu.getEmulator().setExecuteAddress(0x0044c840L);
   boolean frameBoundary=false,returned=false;
   for(int steps=0;steps<100000;steps++){
    long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN){returned=true;break;}
    if(axeGateReturn==pc){int allow=(int)(emu.readRegister("EAX").longValue()&0xff);String event="can:1:"+allow;itemEvents.add(event);orderedEvents.add(event);axeGateReturn=-1;}
    if(pc==AXE_GATE){axeRootCalls++;axeGateReturn=Integer.toUnsignedLong(r(emu.readRegister("ESP").longValue()));}
    if(pc==AXE_CHILD_GATE){long receiver=emu.readRegister("ECX").longValue();if(objects[1]!=0&&receiver==objects[1]+0x200)axePrimaryCalls++;else if(objects[1]!=0&&receiver==objects[1]+0x648)axeSecondaryCalls++;else throw new IllegalStateException("Axe child gate received unexpected object 0x"+Long.toHexString(receiver));}
    if(pc==TRANSFORM){frameBoundary=true;break;}
    if(pc==HUD){long sp=emu.readRegister("ESP").longValue();long receiver=emu.readRegister("ECX").longValue();int code=r(sp+4),parameter=r(sp+8);String event="hud:"+(receiver-PLAYER)+":"+code+":"+parameter;uiEvents.add(event);orderedEvents.add(event);ret(8,0);continue;}
    if(pc==HIDE){long sp=emu.readRegister("ESP").longValue();long receiver=emu.readRegister("ECX").longValue();int parameter=r(sp+4);String event="hide:"+(receiver-PLAYER)+":"+parameter;uiEvents.add(event);orderedEvents.add(event);ret(4,0);continue;}
    if(pc==STUB+0x100||isSlotStub(pc,0x100,0x20)){
     long receiver=emu.readRegister("ECX").longValue();int slot=slotFor(receiver,objects);
     int allow=r(pc);
     String event="can:"+slot+":"+allow;itemEvents.add(event);orderedEvents.add(event);ret(0,allow);continue;
    }
    int activeSlot=slotStub(pc,0x108,0x20);
    if(activeSlot>=0){long sp=emu.readRegister("ESP").longValue();long receiver=emu.readRegister("ECX").longValue();int arg=r(sp+4);String event="active:"+slotFor(receiver,objects)+":"+arg;itemEvents.add(event);orderedEvents.add(event);ret(4,0);continue;}
    if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   }
   if(!frameBoundary&&!returned)throw new IllegalStateException("Owner transaction instruction limit at "+emu.getExecutionAddress());
   int[] slotValues=new int[5];for(int i=0;i<5;i++)slotValues[i]=r(ITEMS+i*4)==0?-1:slotFor(Integer.toUnsignedLong(r(ITEMS+i*4)),objects);
   boolean[] expectedCan=c.canDeactivate.clone();expectedCan[1]=(c.axePrimaryEnabled==0||c.axePrimaryState==0)&&c.axeSecondaryState==0;
   return "{\"name\":\""+c.name+"\",\"action\":"+c.action+",\"selected_before\":"+c.selected+",\"selected_after\":"+r(PLAYER+0x357c)+
   ",\"hose_present\":"+(objects[4]!=0)+",\"hose_mode_before\":"+c.mode+",\"hose_mode_after\":"+(objects[4]==0?0:r(HOSE+0xc68))+
    ",\"hose_valve_pointer_is_root\":"+(objects[4]!=0&&r(HOSE+0xc6c)==(int)VALVE)+",\"stopped_at_frame_transform\":"+frameBoundary+",\"returned\":"+returned+
    ",\"input_slots\":["+ints(c.present)+"],\"axe_primary_enabled\":"+c.axePrimaryEnabled+",\"axe_primary_state\":"+c.axePrimaryState+",\"axe_secondary_state\":"+c.axeSecondaryState+
    ",\"axe_root_gate_calls\":"+axeRootCalls+",\"axe_primary_child_gate_calls\":"+axePrimaryCalls+",\"axe_secondary_child_gate_calls\":"+axeSecondaryCalls+
    ",\"can_deactivate\":["+booleans(expectedCan)+"],\"slots\":["+ints(slotValues)+"],\"item_events\":["+quoted(itemEvents)+"],\"ui_events\":["+quoted(uiEvents)+"],\"ordered_events\":["+quoted(orderedEvents)+"]}";
  }finally{emu.dispose();}
 }
 private String captureEmptyReselect()throws Exception{
  emu=new EmulatorHelper(currentProgram);
  try{
   emu.writeMemory(a(PLAYER),new byte[0x4000]);emu.writeMemory(a(ITEMS),new byte[0x100]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
   for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
   emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);w(PLAYER+0x3564,(int)ITEMS);w(PLAYER+0x3568,5);w(PLAYER+0x357c,4);w(STACK,(int)RETURN);
   emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",PLAYER);emu.getEmulator().setExecuteAddress(0x0044c360L);
   for(int steps=0;steps<10000&&emu.getExecutionAddress().getOffset()!=RETURN;steps++)if(!emu.step(monitor))throw new IllegalStateException("Empty-rescan P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Empty rescan did not return");
   return "{\"name\":\"no_candidates_clears_selection\",\"selected_before\":4,\"selected_after\":"+r(PLAYER+0x357c)+"}";
  }finally{emu.dispose();}
 }
 private boolean isSlotStub(long pc,int offset,int stride){return slotStub(pc,offset,stride)>=0;}
 private int slotStub(long pc,int offset,int stride){if(pc<STUB+offset)return -1;long delta=pc-(STUB+offset);if(delta%stride!=0)return -1;long slot=delta/stride;return slot<5?(int)slot:-1;}
 private int slotFor(long pointer,long[] objects){for(int i=0;i<objects.length;i++)if(objects[i]==pointer)return i;throw new IllegalStateException("Unknown inventory item receiver 0x"+Long.toHexString(pointer));}
 private String ints(int[] values){StringBuilder b=new StringBuilder();for(int i=0;i<values.length;i++){if(i>0)b.append(',');b.append(values[i]);}return b.toString();}
 private String booleans(boolean[] values){StringBuilder b=new StringBuilder();for(int i=0;i<values.length;i++){if(i>0)b.append(',');b.append(values[i]);}return b.toString();}
 private String quoted(List<String> values){StringBuilder b=new StringBuilder();for(int i=0;i<values.size();i++){if(i>0)b.append(',');b.append("\"").append(values.get(i)).append("\"");}return b.toString();}
 private void ret(int argumentBytes,int eax)throws Exception{long sp=emu.readRegister("ESP").longValue();long target=Integer.toUnsignedLong(r(sp));emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+argumentBytes);emu.getEmulator().setExecuteAddress(target);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
}
