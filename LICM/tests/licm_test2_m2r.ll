; ModuleID = './tests/licm_test2_m2r.bc'
source_filename = "./tests/licm_test2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"%d, %d, %d, %d, %d \0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = add nsw i32 20, 1
  %2 = add nsw i32 10, -1
  br label %3

3:                                                ; preds = %31, %0
  %.02 = phi i32 [ 0, %0 ], [ %.3, %31 ]
  %.01 = phi i32 [ 0, %0 ], [ %.1, %31 ]
  %.0 = phi i32 [ 2, %0 ], [ %32, %31 ]
  %4 = icmp slt i32 %.0, %.01
  br i1 %4, label %5, label %33

5:                                                ; preds = %3
  br label %6

6:                                                ; preds = %23, %5
  %.13 = phi i32 [ %.02, %5 ], [ %.2, %23 ]
  %.1 = phi i32 [ 2, %5 ], [ %24, %23 ]
  %7 = sdiv i32 %.0, %.1
  %8 = add nsw i32 %7, 10
  %9 = icmp sle i32 %.1, %8
  br i1 %9, label %10, label %25

10:                                               ; preds = %6
  %11 = add nsw i32 %.13, 100
  %12 = sext i32 %.1 to i64
  %13 = trunc i64 %12 to i32
  br label %14

14:                                               ; preds = %20, %10
  %.2 = phi i32 [ 0, %10 ], [ %21, %20 ]
  %15 = icmp slt i32 %.2, 100
  br i1 %15, label %16, label %22

16:                                               ; preds = %14
  %17 = mul nsw i32 %1, %2
  %18 = add nsw i32 %17, 1
  %19 = add nsw i32 %.2, %.0
  br label %20

20:                                               ; preds = %16
  %21 = add nsw i32 %19, 1
  br label %14, !llvm.loop !6

22:                                               ; preds = %14
  br label %23

23:                                               ; preds = %22
  %24 = add nsw i32 100, 1
  br label %6, !llvm.loop !8

25:                                               ; preds = %6
  %26 = sdiv i32 %.0, %.1
  %27 = icmp sgt i32 %.1, %26
  br i1 %27, label %28, label %30

28:                                               ; preds = %25
  %29 = add nsw i32 %.13, 1
  br label %30

30:                                               ; preds = %28, %25
  %.3 = phi i32 [ %29, %28 ], [ %.13, %25 ]
  br label %31

31:                                               ; preds = %30
  %32 = add nsw i32 %.0, 1
  br label %3, !llvm.loop !9

33:                                               ; preds = %3
  %34 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %.01, i32 noundef %.0, i32 noundef %.02, i32 noundef %1, i32 noundef %2)
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
