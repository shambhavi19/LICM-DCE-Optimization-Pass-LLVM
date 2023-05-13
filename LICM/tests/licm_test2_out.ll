; ModuleID = './tests/licm_test2_out.bc'
source_filename = "./tests/licm_test2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"%d, %d, %d, %d, %d \0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = add nsw i32 20, 1
  %2 = add nsw i32 10, -1
  %3 = icmp slt i32 2, 0
  br i1 %3, label %new_preheader3, label %41

new_preheader3:                                   ; preds = %0
  %4 = mul nsw i32 %1, %2
  %5 = add nsw i32 %4, 1
  %6 = add nsw i32 100, 1
  br label %7

7:                                                ; preds = %38, %new_preheader3
  %.02 = phi i32 [ 0, %new_preheader3 ], [ %.3, %38 ]
  %.01 = phi i32 [ 0, %new_preheader3 ], [ %31, %38 ]
  %.0 = phi i32 [ 2, %new_preheader3 ], [ %39, %38 ]
  br label %8

8:                                                ; preds = %7
  %9 = sdiv i32 %.0, 2
  %10 = add nsw i32 %9, 10
  %11 = icmp sle i32 2, %10
  br i1 %11, label %new_preheader1, label %30

new_preheader1:                                   ; preds = %8
  br label %12

12:                                               ; preds = %26, %new_preheader1
  %.13 = phi i32 [ %.02, %new_preheader1 ], [ %25, %26 ]
  %.1 = phi i32 [ 2, %new_preheader1 ], [ %6, %26 ]
  br label %13

13:                                               ; preds = %12
  %14 = add nsw i32 %.13, 100
  %15 = sext i32 %.1 to i64
  %16 = trunc i64 %15 to i32
  %17 = icmp slt i32 0, 100
  br i1 %17, label %new_preheader, label %24

new_preheader:                                    ; preds = %13
  br label %18

18:                                               ; preds = %21, %new_preheader
  %.2 = phi i32 [ 0, %new_preheader ], [ %22, %21 ]
  br label %19

19:                                               ; preds = %18
  %20 = add nsw i32 %.2, %.0
  br label %21

21:                                               ; preds = %19
  %22 = add nsw i32 %20, 1
  %23 = icmp slt i32 %.2, 100
  br i1 %23, label %18, label %24

24:                                               ; preds = %21, %13
  %25 = phi i32 [ %.2, %21 ], [ 0, %13 ]
  br label %.unified_exit

.unified_exit:                                    ; preds = %24
  br label %26

26:                                               ; preds = %.unified_exit
  %27 = sdiv i32 %.0, %.1
  %28 = add nsw i32 %27, 10
  %29 = icmp sle i32 %.1, %28
  br i1 %29, label %12, label %30

30:                                               ; preds = %26, %8
  %31 = phi i32 [ %.1, %26 ], [ 2, %8 ]
  %32 = phi i32 [ %.13, %26 ], [ %.02, %8 ]
  br label %.unified_exit2

.unified_exit2:                                   ; preds = %30
  %33 = sdiv i32 %.0, %31
  %34 = icmp sgt i32 %31, %33
  br i1 %34, label %35, label %37

35:                                               ; preds = %.unified_exit2
  %36 = add nsw i32 %32, 1
  br label %37

37:                                               ; preds = %35, %.unified_exit2
  %.3 = phi i32 [ %36, %35 ], [ %32, %.unified_exit2 ]
  br label %38

38:                                               ; preds = %37
  %39 = add nsw i32 %.0, 1
  %40 = icmp slt i32 %.0, %.01
  br i1 %40, label %7, label %41

41:                                               ; preds = %38, %0
  %42 = phi i32 [ %.0, %38 ], [ 2, %0 ]
  %43 = phi i32 [ %.01, %38 ], [ 0, %0 ]
  %44 = phi i32 [ %.02, %38 ], [ 0, %0 ]
  br label %.unified_exit4

.unified_exit4:                                   ; preds = %41
  %45 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %43, i32 noundef %42, i32 noundef %44, i32 noundef %1, i32 noundef %2)
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
