; ModuleID = './tests/dominator_test_out.bc'
source_filename = "./tests/dominator_test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  br label %1

1:                                                ; preds = %4, %0
  %.01 = phi i32 [ undef, %0 ], [ %8, %4 ]
  %.0 = phi i32 [ 1, %0 ], [ %2, %4 ]
  %2 = add nsw i32 %.0, 1
  %3 = icmp slt i32 %.0, 100
  br i1 %3, label %4, label %9

4:                                                ; preds = %1
  %5 = add nsw i32 0, 2
  %6 = add nsw i32 %5, 3
  %7 = mul nsw i32 %6, 4
  %8 = sdiv i32 %7, 2
  br label %1, !llvm.loop !6

9:                                                ; preds = %1
  ret i32 %.01
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 15.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
