; ModuleID = './tests/dce_test1.bc'
source_filename = "./tests/dce_test1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %l = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 0, ptr %l, align 4
  store i32 1, ptr %i, align 4
  %0 = load i32, ptr %i, align 4
  %add = add nsw i32 %0, 1
  store i32 %add, ptr %j, align 4
  %1 = load i32, ptr %j, align 4
  %2 = load i32, ptr %i, align 4
  %add1 = add nsw i32 %1, %2
  store i32 %add1, ptr %k, align 4
  %3 = load i32, ptr %l, align 4
  %4 = load i32, ptr %j, align 4
  %add2 = add nsw i32 %3, %4
  store i32 %add2, ptr %l, align 4
  ret i32 2
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
