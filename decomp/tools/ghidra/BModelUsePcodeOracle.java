import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute the original Player-side BModel lift-action gate. */
public class BModelUsePcodeOracle extends GhidraScript {
 private static final long PLAYER=0x30000000L,ITEMS=0x30004000L,HOSE=0x30005000L;
 private static final long BMODEL=0x30007000L,STACK=0x20010000L,RETURN=0x20018000L;
 private EmulatorHelper emu;

 private static class Case {
  String name;boolean hosePresent;int hoseMode,playerByte,heldPointer,objectState;
  Case(String n,boolean hp,int hm,int pb,int held,int os){name=n;hosePresent=hp;hoseMode=hm;playerByte=pb;heldPointer=held;objectState=os;}
 }

 @Override public void run() throws Exception {
  String[] args=getScriptArgs();
  if(args.length!=2)throw new IllegalArgumentException("Usage: BModelUsePcodeOracle.java <output-json> <original-exe>");
  StringBuilder hash=new StringBuilder();
  for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  List<Case> cases=List.of(
   new Case("hose_absent_allowed",false,0,1,0,0),
   new Case("hose_mode_2_allowed",true,2,1,0,0),
   new Case("hose_mode_3_rejected",true,3,1,0,0),
   new Case("player_byte_zero_rejected",false,0,0,0,0),
   new Case("held_object_rejected",false,0,1,1,0),
   new Case("bmodel_state_one_rejected",false,0,1,0,1),
   new Case("bmodel_state_two_rejected",false,0,1,0,2),
   new Case("player_byte_other_nonzero_allowed",false,0,2,0,0));
  List<String> rows=new ArrayList<>();
  for(Case c:cases){int action=capture(c);rows.add("{\"name\":\""+c.name+"\",\"hose_slot_present\":"+c.hosePresent+
   ",\"hose_mode\":"+c.hoseMode+",\"player_byte_12c\":"+c.playerByte+
   ",\"held_pointer_nonnull\":"+(c.heldPointer!=0)+",\"bmodel_state\":"+c.objectState+
   ",\"action\":"+Integer.toUnsignedString(action)+"}");}
  int[] transition=captureLiftTransition();
  String transitionRow="{\"name\":\"mode1_enter_lift\",\"player_held_object_is_bmodel\":"+(transition[0]==1)+
   ",\"bmodel_state\":"+transition[1]+",\"player_state\":"+transition[2]+",\"helper_called\":"+(transition[3]!=0)+
   ",\"helper_receiver_offset\":"+transition[4]+",\"helper_argument_1\":"+transition[5]+",\"helper_argument_2\":"+Integer.toUnsignedString(transition[6])+
   ",\"held_object_at_helper_is_bmodel\":"+(transition[7]==1)+",\"bmodel_state_at_helper\":"+transition[8]+",\"player_state_at_helper\":"+transition[9]+"}";
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"0044C520,0044BC40\",\"scope\":\"Player lift action gate plus mode-1 field transition; 004571A0 is stopped at its call boundary and recorded as a controlled service stub; no live field capture\",\"samples\":["+String.join(",",rows)+"],\"transitions\":["+transitionRow+"]}\n");
  println("Captured "+rows.size()+" original BModel lift-action gate cases and the mode-1 transition");
 }

 private int capture(Case c)throws Exception{
  emu=new EmulatorHelper(currentProgram);
  try{
   emu.writeMemory(address(PLAYER),new byte[0x4000]);
   emu.writeMemory(address(ITEMS),new byte[0x100]);
   emu.writeMemory(address(HOSE),new byte[0xd00]);
   emu.writeMemory(address(BMODEL),new byte[0x400]);
   emu.writeMemory(address(STACK),new byte[0x100]);
   for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
   emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
   write(PLAYER+0x3564,(int)ITEMS);write(PLAYER+0x3568,5);
   if(c.hosePresent){write(ITEMS+0x10,(int)HOSE);write(HOSE+0xc68,c.hoseMode);}
   write(PLAYER+0x3440,c.heldPointer);write(BMODEL+0x25c,c.objectState);
   emu.writeMemoryValue(address(PLAYER+0x12c),1,c.playerByte);
   emu.writeMemoryValue(address(STACK),4,RETURN);emu.writeMemoryValue(address(STACK+4),4,BMODEL);
   Address stop=address(RETURN);emu.setBreakpoint(stop);
   emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",PLAYER);emu.writeRegister(emu.getPCRegister(),0x0044c520L);
   if(!emu.run(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   if(!emu.getExecutionAddress().equals(stop))throw new IllegalStateException("Player gate stopped at "+emu.getExecutionAddress());
   return (int)(emu.readRegister("EAX").longValue()&0xffffffffL);
  }finally{emu.dispose();}
 }

 private int[] captureLiftTransition()throws Exception{
  emu=new EmulatorHelper(currentProgram);
  try{
   emu.writeMemory(address(PLAYER),new byte[0x4000]);
   emu.writeMemory(address(BMODEL),new byte[0x400]);
   emu.writeMemory(address(STACK-0x100),new byte[0x300]);
   for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
   emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
   write(PLAYER+0x18c,3);write(PLAYER+0x3440,0);write(BMODEL+0x25c,0);
   emu.writeMemoryValue(address(STACK),4,RETURN);emu.writeMemoryValue(address(STACK+4),4,BMODEL);
   Address stop=address(RETURN);boolean helperCalled=false;int helperReceiver=0,helperArg1=0,helperArg2=0;
   int heldBefore=0,stateBefore=0,objectBefore=0;
   emu.setBreakpoint(stop);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",PLAYER);emu.writeRegister(emu.getPCRegister(),0x0044bc40L);
   for(int steps=0;steps<10000;steps++){
    Address pc=emu.getExecutionAddress();if(pc.equals(stop))break;
    if(pc.equals(address(0x004571a0L))){
     helperCalled=true;helperReceiver=(int)(emu.readRegister("ECX").longValue()-PLAYER);
     long sp=emu.readRegister("ESP").longValue();long returnPc=Integer.toUnsignedLong(read(sp));
     helperArg1=read(sp+4);helperArg2=read(sp+8);heldBefore=read(PLAYER+0x3440);
     stateBefore=read(PLAYER+0x18c);objectBefore=read(BMODEL+0x25c);
     emu.writeRegister("EAX",0);emu.writeRegister("ESP",sp+12);emu.getEmulator().setExecuteAddress(returnPc);
     continue;
    }
    if(!emu.step(monitor))throw new IllegalStateException("Lift transition P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
   }
   if(!emu.getExecutionAddress().equals(stop))throw new IllegalStateException("Lift transition did not return; pc="+emu.getExecutionAddress());
   return new int[]{read(PLAYER+0x3440)==(int)BMODEL?1:0,read(BMODEL+0x25c),read(PLAYER+0x18c),
    helperCalled?1:0,helperReceiver,helperArg1,helperArg2,heldBefore==(int)BMODEL?1:0,objectBefore,stateBefore};
  }finally{emu.dispose();}
 }

 private void write(long at,int value)throws Exception{emu.writeMemoryValue(address(at),4,Integer.toUnsignedLong(value));}
 private int read(long at)throws Exception{byte[]b=emu.readMemory(address(at),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private Address address(long value){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(value);}
}
