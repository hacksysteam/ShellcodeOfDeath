ShellcodeOfDeath
================


                      _     __             _____                    
      /\  /\__ _  ___| | __/ _\_   _ ___  /__   \___  __ _ _ __ ___  
     / /_/ / _` |/ __| |/ /\ \| | | / __|   / /\/ _ \/ _` | '_ ` _ \
    / __  / (_| | (__|   < _\ \ |_| \__ \  / / |  __/ (_| | | | | | |
    \/ /_/ \__,_|\___|_|\_\\__/\__, |___/  \/   \___|\__,_|_| |_| |_|
                               |___/                                
                                    http://hacksys.vfreaks.com/
                                      hacksysteam@hotmail.com

                       Shellcode Of Death
                                        v1.0
  
          Ashfaq Ansari (ashfaq_ansari1989@hotmail.com)
          Ruei-Min Jiang (@mike820324) a.k.a MicroMike

 Still a lot can be done to improve this shellcode. This is the first release.
 Further, a lot of improvement and optimization can be done to this shellcode.
 Any bugs, suggestions or contribution is most welcomed.

 Lenth of shellcode: 387 bytes
 Number of NULL byte: 49

 Notes: There are more NULL bytes due to a fact that few of the Windows API used
        accepts input in UNICODE format. Still, NULL bytes can be reduces further.
        We can use any simple encoder to xor encode the shellcode and eventually
        eliminate NULL bytes.
               
 Things to do:
              1. Code cleanup
              2. Optimization
              3. Minification
              4. Add suggestions
