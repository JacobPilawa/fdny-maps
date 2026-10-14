import ghidra.program.model.address.Address;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.CodeUnitIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.util.task.TaskMonitor;

public class ScanBModelStrikeField extends ghidra.app.script.GhidraScript {
    @Override
    public void run() throws Exception {
        println("PROGRAM " + currentProgram.getName());
        println("IMAGE_BASE " + currentProgram.getImageBase());
        CodeUnitIterator units = currentProgram.getListing().getCodeUnits(true);
        while (units.hasNext() && !monitor.isCancelled()) {
            CodeUnit unit = units.next();
            if (!(unit instanceof Instruction)) continue;
            Instruction instruction = (Instruction) unit;
            String text = instruction.toString().toLowerCase();
            if (!text.matches(".*(?<![0-9a-f])0?1ec(?![0-9a-f]).*")) continue;
            Address address = instruction.getAddress();
            Function function = currentProgram.getFunctionManager().getFunctionContaining(address);
            println((function == null ? "<no function>" : function.getEntryPoint() + " " + function.getName()) +
                    " | " + address + " | " + instruction);
        }
    }
}
