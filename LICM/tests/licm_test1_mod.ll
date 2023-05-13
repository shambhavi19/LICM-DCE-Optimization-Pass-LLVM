; ModuleID = './tests/licm_test1_mod.bc'
source_filename = "./tests/licm_test1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [15 x i8] c"result is: %d\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @test(i32 noundef %0) #0 {
  %2 = icmp slt i32 0, 100
  br i1 %2, label %new_preheader, label %10

new_preheader:                                    ; preds = %1
  br label %3

3:                                                ; preds = %7, %new_preheader
  %.01 = phi i32 [ 0, %new_preheader ], [ %5, %7 ]
  %.0 = phi i32 [ 0, %new_preheader ], [ %8, %7 ]
  br label %4

4:                                                ; preds = %3
  %5 = mul nsw i32 %.01, 2
  %6 = add nsw i32 %0, 3
  br label %7

7:                                                ; preds = %4
  %8 = add nsw i32 %.0, 1
  %9 = icmp slt i32 %.0, 100
  br i1 %9, label %3, label %10

10:                                               ; preds = %7, %1
  %11 = phi i32 [ %.0, %7 ], [ 0, %1 ]
  %12 = phi i32 [ %.01, %7 ], [ 0, %1 ]
  br label %.unified_exit

.unified_exit:                                    ; preds = %10
  ret i32 %12
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = call i32 @test(i32 noundef 3)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %1)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 15.0.6"}
