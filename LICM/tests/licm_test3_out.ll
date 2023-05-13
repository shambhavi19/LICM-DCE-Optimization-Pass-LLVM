; ModuleID = './tests/licm_test3_out.bc'
source_filename = "./tests/licm_test3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = icmp slt i32 0, 100
  br i1 %1, label %new_preheader1, label %27

new_preheader1:                                   ; preds = %0
  %2 = mul nsw i32 5, 5
  br label %3

3:                                                ; preds = %24, %new_preheader1
  %.03 = phi i32 [ 0, %new_preheader1 ], [ %23, %24 ]
  %.02 = phi i32 [ 0, %new_preheader1 ], [ %25, %24 ]
  %.01 = phi i32 [ undef, %new_preheader1 ], [ %22, %24 ]
  br label %4

4:                                                ; preds = %3
  %5 = add nsw i32 %.02, 5
  %6 = add nsw i32 %.03, %5
  %7 = icmp ne i32 %6, 0
  br i1 %7, label %8, label %9

8:                                                ; preds = %4
  br label %10

9:                                                ; preds = %4
  br label %10

10:                                               ; preds = %9, %8
  %11 = icmp slt i32 0, 10
  br i1 %11, label %new_preheader, label %20

new_preheader:                                    ; preds = %10
  br label %12

12:                                               ; preds = %17, %new_preheader
  %.14 = phi i32 [ %6, %new_preheader ], [ %16, %17 ]
  %.1 = phi i32 [ 5, %new_preheader ], [ 10, %17 ]
  %.0 = phi i32 [ 0, %new_preheader ], [ %18, %17 ]
  br label %13

13:                                               ; preds = %12
  %14 = add nsw i32 %.0, 5
  %15 = add nsw i32 %.14, %14
  %16 = add nsw i32 %15, %2
  br label %17

17:                                               ; preds = %13
  %18 = add nsw i32 %.0, 1
  %19 = icmp slt i32 %.0, 10
  br i1 %19, label %12, label %20

20:                                               ; preds = %17, %10
  %21 = phi i32 [ %.0, %17 ], [ 0, %10 ]
  %22 = phi i32 [ %.1, %17 ], [ 5, %10 ]
  %23 = phi i32 [ %.14, %17 ], [ %6, %10 ]
  br label %.unified_exit

.unified_exit:                                    ; preds = %20
  br label %24

24:                                               ; preds = %.unified_exit
  %25 = add nsw i32 %.02, 1
  %26 = icmp slt i32 %.02, 100
  br i1 %26, label %3, label %27

27:                                               ; preds = %24, %0
  %28 = phi i32 [ %.01, %24 ], [ undef, %0 ]
  %29 = phi i32 [ %.02, %24 ], [ 0, %0 ]
  %30 = phi i32 [ %.03, %24 ], [ 0, %0 ]
  br label %.unified_exit2

.unified_exit2:                                   ; preds = %27
  %31 = add nsw i32 6, 2
  %32 = add nsw i32 %28, 4
  ret i32 %30
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
