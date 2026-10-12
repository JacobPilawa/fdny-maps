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

/** Execute original 00446470 and its actual descriptor reset/RNG helpers. */
public class AshParticleActivatePcodeOracle extends GhidraScript {
    private static final long PARTICLE=0x20000000L, INPUT=0x20002000L;
    private static final long DESCRIPTOR=0x20003000L, STACK=0x20fff000L, RETURN=0x70000000L;
    private static final long SEED=0x00404b50L, ACTIVATE=0x00446470L, RANGE=0x00498b30L;

    private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
    private void w(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
    private int r(EmulatorHelper e,long p)throws Exception{
        byte[] b=e.readMemory(a(p),4);
        return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);
    }
    private int byteAt(EmulatorHelper e,long p)throws Exception{return e.readMemory(a(p),1)[0]&255;}
    private void zeroCpu(EmulatorHelper e)throws Exception{
        for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})e.writeRegister(n,0);
        e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);
        Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)e.writeRegister(cw,0x037fL);
        Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)e.writeRegister(sw,0);
        Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)e.writeRegister(tag,0xffffL);
        for(Register reg:currentProgram.getLanguage().getRegisters())
            if(reg.getName().toUpperCase().matches("ST[0-7]"))e.writeRegister(reg,BigInteger.ZERO);
    }
    private void runToReturn(EmulatorHelper e,long entry,int maxSteps,String label)throws Exception{
        e.getEmulator().setExecuteAddress(entry);
        for(int i=0;i<maxSteps&&e.getExecutionAddress().getOffset()!=RETURN;i++)
            if(!e.step(monitor))throw new IllegalStateException(label+" P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
        if(e.getExecutionAddress().getOffset()!=RETURN)
            throw new IllegalStateException(label+" instruction limit at "+e.getExecutionAddress());
    }
    private String sample(String name,int seed,int now,int xBits,int yBits,int zBits)throws Exception{
        EmulatorHelper e=new EmulatorHelper(currentProgram);
        try{
            e.writeMemory(a(PARTICLE),new byte[0x200]);
            e.writeMemory(a(INPUT),new byte[0x20]);
            e.writeMemory(a(DESCRIPTOR),new byte[0x40]);
            e.writeMemory(a(STACK),new byte[0x100]);
            w(e,DESCRIPTOR+4,5);
            w(e,PARTICLE+0x0c,(int)DESCRIPTOR);
            w(e,PARTICLE+0x04,9);w(e,PARTICLE+0x08,9);w(e,PARTICLE+0x14,0x87654321);
            w(e,PARTICLE+0x18,0x3c);w(e,PARTICLE+0x1c,0x3f800000);
            w(e,PARTICLE+0x30,0);w(e,PARTICLE+0x78,0);w(e,PARTICLE+0xa0,0);
            w(e,PARTICLE+0xa8,0x3f800000);w(e,PARTICLE+0x168,0x12345678);
            w(e,INPUT, xBits);w(e,INPUT+4,yBits);w(e,INPUT+8,zBits);
            w(e,0x00502afcL,now);
            zeroCpu(e);w(e,STACK,(int)RETURN);w(e,STACK+4,seed);
            e.writeRegister("ESP",STACK);e.writeRegister("ECX",PARTICLE);
            runToReturn(e,SEED,20000,"RNG seed");

            zeroCpu(e);w(e,0x00502afcL,now);w(e,STACK,(int)RETURN);w(e,STACK+4,(int)INPUT);
            e.writeRegister("ESP",STACK);e.writeRegister("ECX",PARTICLE);
            e.getEmulator().setExecuteAddress(ACTIVATE);
            int rangeCalls=0;
            for(int steps=0;steps<20000&&e.getExecutionAddress().getOffset()!=RETURN;steps++){
                if(e.getExecutionAddress().getOffset()==RANGE)rangeCalls++;
                if(!e.step(monitor))throw new IllegalStateException("Activation P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
            }
            if(e.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Activation instruction limit at "+e.getExecutionAddress());
            return "{\"name\":\""+name+"\",\"seed\":"+Integer.toUnsignedString(seed)+
                ",\"now\":"+Integer.toUnsignedString(now)+
                ",\"position_before\":["+Integer.toUnsignedLong(xBits)+","+Integer.toUnsignedLong(yBits)+","+Integer.toUnsignedLong(zBits)+"]"+
                ",\"active_after\":"+(byteAt(e,PARTICLE+0x30)!=0)+
                ",\"position_initialized_after\":"+(byteAt(e,PARTICLE+0x78)!=0)+
                ",\"dirty_after\":"+(byteAt(e,PARTICLE+0xa0)!=0)+
                ",\"animation_index_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+4))+
                ",\"next_animation_index_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+8))+
                ",\"animation_timestamp_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0x14))+
                ",\"activation_scale_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0x1c))+
                ",\"spawn_position_after\":["+Integer.toUnsignedLong(r(e,PARTICLE+0x5c))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x60))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x64))+"]"+
                ",\"opacity_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0xa8))+
                ",\"deadline_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0x168))+
                ",\"vector7c_after\":["+Integer.toUnsignedLong(r(e,PARTICLE+0x7c))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x80))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x84))+"]"+
                ",\"range_calls\":"+rangeCalls+"}";
        }finally{e.dispose();}
    }
    @Override public void run()throws Exception{
        String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshParticleActivatePcodeOracle.java output.json original.exe");
        StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
        if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
        int[][] positions={{0x41280000,0xc1a20000,0x40400000},{0x3f800001,0x80000000,0x3f800003},{0xc7700000,0x41c80000,0x45fa0000}};
        int[] seeds={0x1234,0x13579bdf,0xabcdef01};int[] clocks={1,0x10203040,0xffffff00};
        List<String> rows=new ArrayList<>();
        for(int i=0;i<positions.length;i++)rows.add(sample("activate_"+i,seeds[i],clocks[i],positions[i][0],positions[i][1],positions[i][2]));
        Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00446470\",\"rng_seed_entry\":\"00404B50\",\"range_entry\":\"00498B30\",\"scope\":\"Activation function plus original animation-index validation, vector access/store, and bounded RNG; scene transform before entry and presentation are outside scope\",\"samples\":["+String.join(",",rows)+"]}\n");
        println("Captured "+rows.size()+" original ash activation cases");
    }
}
