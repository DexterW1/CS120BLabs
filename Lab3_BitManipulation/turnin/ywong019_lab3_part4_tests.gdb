# Test file for "Lab3_BitManipulation"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Example test:
#test "PINA: 0x00, PINB: 0x00 => PORTC: 0"
# Set inputs
#setPINA 0x00
#setPINB 0x00
# Continue for several ticks
#continue 2
# Set expect values
#expectPORTC 0
# Check pass/fail
#checkResult

# Add tests below
test(1) "PINA: 0x25 =>PORTB:0x02 && PORTC:0x50"
setPINA 0x25
continue 5
expectPORTB 0x02
expectPORTC 0x50
checkResult
#test(2)
test(2) "PINA: 0xBF => PORTB: 0x0B && PORTC: 0xF0"
setPINA 0xBF
continue 5
expectPORTB 0x0B
expectPORTC 0xF0
checkResult
#test(3)
test(3) "PINA: 0x18 => PORTB:0x01  && PORTC: 0x80"
setPINA 0x18
continue 5
expectPORTB 0x01
expectPORTC 0x80
checkResult
#test(4)
test(4) "PINA: 0x90 => PORTB:0x09 && PORTC: 0x00"
setPINA 0x90
continue 5
expectPORTB 0x09
expectPORTC 0x00
checkResult
#test(5)
test(5) "PINA: 0x09 => PORTB:0x00 && PORTC: 0x90"
setPINA 0x09
continue 5
expectPORTB 0x00
expectPORTC 0x90
checkResult






# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
