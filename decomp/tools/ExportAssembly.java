// @category FDNY
import java.io.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;

public class ExportAssembly extends GhidraScript {
  public void run() throws Exception {
    String[] args = getScriptArgs();
    try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
      for (int n=1; n<args.length; n++) {
        Address address = toAddr(args[n].replace("0x", ""));
        Function f = getFunctionContaining(address);
        if (f == null && currentProgram.getMemory().getBlock(address) != null
                && currentProgram.getMemory().getBlock(address).isExecute()) {
          disassemble(address);
          f = createFunction(address, null);
        }
        if (f == null) throw new IllegalArgumentException("No function at " + args[n]);
        out.println("FUNCTION " + f.getEntryPoint() + " " + f.getName());
        InstructionIterator it = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (it.hasNext()) { Instruction i = it.next(); out.println(i.getAddress() + "  " + i.toString()); }
      }
    }
  }
}
