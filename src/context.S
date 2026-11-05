// context_switch(*a_ctx, *b_ctx)
//
// we were called from thread A attempting to switch back to thread B
// the biggest thing is that we have to load B's x30 and sp back so that we pop out into B
// effectively once we do that it will be as if context_switch() just returned from B's perspective,
// and we will be back in b's stack frame
//
// Everything else in the context is the callee saved registers of whoever called us that we must put back.




.global context_switch

context_switch:
  stp x19, x20, [x0, #0]
  stp x21, x22, [x0, #16]
  stp x23, x24, [x0, #32]
  stp x25, x26, [x0, #48]
  stp x27, x28, [x0, #64]
  stp x29, x30, [x0, #80]
  mov x2, sp
  str x2,       [x0, #96]

  ldp x19, x20, [x1, #0]
  ldp x21, x22, [x1, #16]
  ldp x23, x24, [x1, #32]
  ldp x25, x26, [x1, #48]
  ldp x27, x28, [x1, #64]
  ldp x29, x30, [x1, #80]
  ldr x2,       [x1, #96]
  mov sp, x2

  ret


