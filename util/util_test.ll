; ModuleID = 'util_test.cpp'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.memsetTest = type { [8 x i32] }

@meme = global [8 x %struct.memsetTest] zeroinitializer, align 16
@bbb = global i32 11, align 4

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** nocapture readnone %args) #0 {
  %1 = load volatile i32* @bbb, align 4, !tbaa !1
  br label %2

; <label>:2                                       ; preds = %0, %7
  %i.01 = phi i32 [ 0, %0 ], [ %8, %7 ]
  %3 = tail call i32 asm "btl $2, $1\0A\09adcl $0, $0\0A\09", "=r,r,ri,0,~{cc},~{dirflag},~{fpsr},~{flags}"(i32 %1, i32 %i.01, i32 0) #1, !srcloc !5
  %4 = tail call i32 asm "btl $2, $1\0A\09adcl $0, $0\0A\09", "=r,r,ri,0,~{cc},~{dirflag},~{fpsr},~{flags}"(i32 %i.01, i32 2, i32 0) #1, !srcloc !5
  %5 = and i32 %4, %3
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %10

; <label>:7                                       ; preds = %2
  %8 = add nuw nsw i32 %i.01, 1
  %9 = icmp ult i32 %8, 32
  br i1 %9, label %2, label %10

; <label>:10                                      ; preds = %2, %7
  %i.0.lcssa = phi i32 [ %i.01, %2 ], [ %8, %7 ]
  %11 = icmp ugt i32 %i.0.lcssa, 13
  %12 = zext i1 %11 to i32
  ret i32 %12
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.6.1-svn232753-1~exp1 (branches/release_36) (based on LLVM 3.6.1)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"int", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C/C++ TBAA"}
!5 = !{i32 944, i32 968, i32 1003}
