void __declspec(naked) HealthFunc()
{
	__asm
	{
	newmem:
		cmp dword ptr[edi+0x12C],0                            // Check if entity struct has stasis cap value
		je Entity                                             // If no stasis cap then jump to entity

	Player:										              
		cmp DeadSpaceOffsets::dwGodmode,0                     // Check if user wants godmode enabled
		je code                                               // If not then proceed to regular code
		push eax                                              // Push EAX
		mov eax,[edi+0x11C]                                   // Move max health cap into EAX
		movd xmm0,eax                                         // Move EAX into XMM0
		pop eax                                               // Pop EAX
		jmp code                                              // Continue to regular code flow

	Entity:										              
		cmp DeadSpaceOffsets::dwOneShotKill,0                 // Check if user wants oneshotkill
		je code                                               // If not then proceed to regular code
		push eax                                              // Push EAX
		mov eax,0                                             // Move 0 into EAX 
		movd xmm0,eax                                         // Move EAX into XMM0
		pop eax                                               // Pop EAX
		jmp code                                              // Continue to regular code flow

	code:
		movss[edi+0x120],xmm0                                 // Move XMM0 into health value
		jmp[DeadSpaceOffsets::HeathDecremenntJumpBackAddr]    // Return to game code
	}
}
