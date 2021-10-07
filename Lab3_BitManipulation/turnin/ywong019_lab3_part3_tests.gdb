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
test(1) "PINA: 0x01 =>PORTC: 0x60"
setPINA 0x01
continue 5
expectPORTC 0x60
checkResult
#test(2)
test(2) "PINA: 0x03 => (1)PORTC: 0x70"
setPINA 0x03
continue 5
expectPORTC 0x70
checkResult
#test(3)
test(3) "PINA: 0x06 => (1)PORTC: 0x38"
setPINA 0x06
continue 5
expectPORTC 0x38
checkResult
#test(4)
test(4) "PINA: 0x08 => (1)PORTC: 0x3C"
setPINA 0x08
continue 5
expectPORTC 0x3C
checkResult
#test(5)
test(5) "PINA: 0x0B  => (1)PORTC: 0x3E"
setPINA 0x0B
continue 5
expectPORTC 0x3E
checkResult
#test(6)
test(6) "PINA: 0x0E  => (1)PORTC: 0x3F"
setPINA 0x0E
continue 5
expectPORTC 0x3F
checkResult
#test(7)
test(7) "PINA: 0x30  => (1)PORTC: 0xC0"
setPINA 0x30
continue 5
expectPORTC 0xC0
checkResult
#test(8)
test(8) "PINA: 0x70  => (1)PORTC: 0x40"
setPINA 0x70
continue 5
expectPORTC 0x40
checkResult







# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
