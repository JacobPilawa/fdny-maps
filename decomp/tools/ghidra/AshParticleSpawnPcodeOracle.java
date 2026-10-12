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

/** Execute the original one-slot 004467E0 spawn path and its real helpers. */
public class AshParticleSpawnPcodeOracle extends GhidraScript {
    private static final long MANAGER=0x20000000L, PARTICLE=0x20001000L;
    private static final long PARTICLE_ARRAY=0x20002000L, PLAYER=0x20003000L;
    private static final long DESCRIPTOR=0x20004000L, STACK=0x20fff000L, RETURN=0x70000000L;
    private static final long SEED=0x00404b50L, EMIT=0x004467e0L, RANGE=0x00498b30L, PLAYER_GETTER=0x0049d760L;

    private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
    private void w(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
    private int r(EmulatorHelper e,long p)throws Exception{
        byte[] b=e.readMemory(a(p),4);
        return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);
    }
    private int b(EmulatorHelper e,long p)throws Exception{return e.readMemory(a(p),1)[0]&255;}
    private double x87Double(BigInteger value){
        BigInteger sig=value.and(BigInteger.ONE.shiftLeft(64).subtract(BigInteger.ONE));
        int signExp=value.shiftRight(64).intValue()&0xffff,exponent=signExp&0x7fff;
        if(exponent==0)return 0.0;
        double significand=new java.math.BigDecimal(sig).doubleValue()/0x1.0p63;
        double result=Math.scalb(significand,exponent-16383);
        return (signExp&0x8000)!=0?-result:result;
    }
    private BigInteger x87Value(double value){
        long bits=Double.doubleToRawLongBits(value);int exp=(int)((bits>>>52)&0x7ff);
        long fraction=bits&0xfffffffffffffL;int sign=(bits<0)?0x8000:0;
        if(exp==0)return BigInteger.ZERO;
        long significand=(fraction|0x10000000000000L)<<11;
        int signExp=sign|((exp-1023+16383)&0x7fff);
        return BigInteger.valueOf(signExp).shiftLeft(64).or(new BigInteger(Long.toUnsignedString(significand)));
    }
    private boolean emulateTrig(EmulatorHelper e)throws Exception{
        long pc=e.getExecutionAddress().getOffset();long next;
        if(pc==0x0044694cL)next=0x0044694eL;
        else if(pc==0x00446969L)next=0x0044696bL;
        else return false;
        double angle=x87Double(e.readRegister("ST0"));
        double result=pc==0x0044694cL?Math.cos(angle):Math.sin(angle);
        e.writeRegister("ST0",x87Value(result));
        e.getEmulator().setExecuteAddress(next);
        return true;
    }
    private void zeroCpu(EmulatorHelper e)throws Exception{
        for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})e.writeRegister(n,0);
        e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);
        Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)e.writeRegister(cw,0x037fL);
        Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)e.writeRegister(sw,0);
        Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)e.writeRegister(tag,0xffffL);
        for(Register reg:currentProgram.getLanguage().getRegisters())
            if(reg.getName().toUpperCase().matches("ST[0-7]"))e.writeRegister(reg,BigInteger.ZERO);
    }
    private void run(EmulatorHelper e,long entry,int maxSteps,String label,int[] rangeCalls,int[] getterCalls)throws Exception{
        e.getEmulator().setExecuteAddress(entry);
        for(int i=0;i<maxSteps&&e.getExecutionAddress().getOffset()!=RETURN;i++){
            if(e.getExecutionAddress().getOffset()==RANGE)rangeCalls[0]++;
            if(e.getExecutionAddress().getOffset()==PLAYER_GETTER)getterCalls[0]++;
            if(emulateTrig(e))continue;
            if(!e.step(monitor))throw new IllegalStateException(label+" P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
        }
        if(e.getExecutionAddress().getOffset()!=RETURN)
            throw new IllegalStateException(label+" instruction limit at "+e.getExecutionAddress());
    }
    private String sample(String name,int seed,int now,int[] position,int[] direction)throws Exception{
        EmulatorHelper e=new EmulatorHelper(currentProgram);
        try{
            e.writeMemory(a(MANAGER),new byte[0x100]);e.writeMemory(a(PARTICLE),new byte[0x200]);
            e.writeMemory(a(PARTICLE_ARRAY),new byte[0x20]);e.writeMemory(a(PLAYER),new byte[0x100]);
            e.writeMemory(a(DESCRIPTOR),new byte[0x40]);e.writeMemory(a(STACK),new byte[0x100]);
            w(e,MANAGER+0x38,(int)PARTICLE_ARRAY);w(e,MANAGER+0x3c,1);
            w(e,MANAGER+0x50,0);w(e,MANAGER+0x54,0x3f800000);w(e,MANAGER+0x58,(int)PLAYER);
            w(e,PARTICLE_ARRAY,(int)PARTICLE);w(e,PARTICLE+0x0c,(int)DESCRIPTOR);w(e,DESCRIPTOR+4,5);
            w(e,PARTICLE+0x30,0);w(e,PARTICLE+0xad,0);w(e,PARTICLE+0xa0,0);
            w(e,PLAYER,0x004e1c48); // primary Player vtable; slot +0x18 is 0049D760
            // 0049D760 returns Player+0x24. Frame direction is +0x1c..+0x24;
            // position is +0x28,+0x2c,+0x30.
            w(e,PLAYER+0x40,direction[0]);w(e,PLAYER+0x44,direction[1]);w(e,PLAYER+0x48,direction[2]);
            w(e,PLAYER+0x4c,position[0]);w(e,PLAYER+0x50,position[1]);w(e,PLAYER+0x54,position[2]);
            w(e,0x00502afcL,now);
            zeroCpu(e);w(e,STACK,(int)RETURN);w(e,STACK+4,seed);e.writeRegister("ESP",STACK);
            run(e,SEED,20000,"RNG seed",new int[]{0},new int[]{0});
            zeroCpu(e);w(e,0x00502afcL,now);w(e,STACK,(int)RETURN);e.writeRegister("ESP",STACK);e.writeRegister("ECX",MANAGER);
            int[] calls={0},getterCalls={0};run(e,EMIT,200000,"ash emitter",calls,getterCalls);
            return "{\"name\":\""+name+"\",\"seed\":"+Integer.toUnsignedString(seed)+
                ",\"now\":"+Integer.toUnsignedString(now)+
                ",\"position_before\":["+Integer.toUnsignedLong(position[0])+","+Integer.toUnsignedLong(position[1])+","+Integer.toUnsignedLong(position[2])+
                "],\"direction_before\":["+Integer.toUnsignedLong(direction[0])+","+Integer.toUnsignedLong(direction[1])+","+Integer.toUnsignedLong(direction[2])+
                "],\"spawn_position_after\":["+Integer.toUnsignedLong(r(e,PARTICLE+0x5c))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x60))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x64))+
                "],\"active_after\":"+(b(e,PARTICLE+0x30)!=0)+
                ",\"activation_scale_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0x1c))+
                ",\"opacity_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0xa8))+
                ",\"deadline_after\":"+Integer.toUnsignedLong(r(e,PARTICLE+0x168))+
                ",\"vector7c_after\":["+Integer.toUnsignedLong(r(e,PARTICLE+0x7c))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x80))+","+Integer.toUnsignedLong(r(e,PARTICLE+0x84))+
                "],\"range_calls\":"+calls[0]+",\"player_getter_calls\":"+getterCalls[0]+"}";
        }finally{e.dispose();}
    }
    @Override public void run()throws Exception{
        String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshParticleSpawnPcodeOracle.java output.json original.exe");
        StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
        if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
        int[][] positions={{0x41280000,0xc1a20000,0x40400000},{0x3f800001,0x80000000,0x3f800003},{0xc7700000,0x41c80000,0x45fa0000},{0,0,0},{0x461c4000,0xc61c4000,0x3eaaaaab}};
        int[][] directions={{0x3f800000,0x40000000,0x40400000},{0x80000000,0x3f000001,0xbf800003},{0x3eaaaaab,0xc1200000,0x42160000},{0,0,0},{0x3f800001,0x80000000,0x3f000001}};
        int[] seeds={0x1234,0x13579bdf,0xabcdef01,0x10203040,0xffffff00};
        int[] clocks={1,0x10203040,0xffffff00,0x80000000,0xffffffff};
        List<String> rows=new ArrayList<>();for(int i=0;i<seeds.length;i++)rows.add(sample("spawn_"+i,seeds[i],clocks[i],positions[i],directions[i]));
        Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"004467E0\",\"rng_seed_entry\":\"00404B50\",\"range_entry\":\"00498B30\",\"method\":\"Original instructions, RNG, virtual Player getter and activation execute in Ghidra P-code; FCOS/FSIN callother ops are supplied with Java Math double results because this Ghidra x86 P-code emulator does not implement those two x87 operators\",\"scope\":\"One inactive particle; original 004467E0 angle/distance and spawn transform, four real Player vtable getter dispatches, original 00446470 activation; other pool slots, live renderer, dynamic side effects beyond leaf 0049D760 and whole-frame schedule are excluded\",\"samples\":["+String.join(",",rows)+"]}\n");
        println("Captured "+rows.size()+" original ash emitter spawn cases");
    }
}
