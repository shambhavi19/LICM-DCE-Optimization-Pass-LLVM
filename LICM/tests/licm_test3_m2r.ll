; ModuleID = './tests/licm_test3_m2r.bc'
source_filename = "./tests/licm_test3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  br label %1

1:                                                ; preds = %20, %0
  %.03 = phi i32 [ 0, %0 ], [ %.14, %20 ]
  %.02 = phi i32 [ 0, %0 ], [ %21, %20 ]
  %.01 = phi i32 [ undef, %0 ], [ %.1, %20 ]
  %2 = icmp slt i32 %.02, 100
  br i1 %2, label %3, label %22

3:                                                ; preds = %1
  %4 = add nsw i32 %.02, 5
  %5 = add nsw i32 %.03, %4
  %6 = icmp ne i32 %5, 0
  br i1 %6, label %7, label %8

7:                                                ; preds = %3
  br label %9

8:                                                ; preds = %3
  br label %9

9:                                                ; preds = %8, %7
  %10 = mul nsw i32 5, 5
  br label %11

11:                                               ; preds = %17, %9
  %.14 = phi i32 [ %5, %9 ], [ %16, %17 ]
  %.1 = phi i32 [ 5, %9 ], [ 10, %17 ]
  %.0 = phi i32 [ 0, %9 ], [ %18, %17 ]
  %12 = icmp slt i32 %.0, 10
  br i1 %12, label %13, label %19

13:                                               ; preds = %11
  %14 = add nsw i32 %.0, 5
  %15 = add nsw i32 %.14, %14
  %16 = add nsw i32 %15, %10
  br label %17

17:                                               ; preds = %13
  %18 = add nsw i32 %.0, 1
  br label %11, !llvm.loop !6

19:                                               ; preds = %11
  br label %20

20:                                               ; preds = %19
  %21 = add nsw i32 %.02, 1
  br label %1, !llvm.loop !8

22:                                               ; preds = %1
  %23 = add nsw i32 6, 2
  %24 = add nsw i32 %.01, 4
  ret i32 %.03
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
!8 = distinct !{!8, !7}
