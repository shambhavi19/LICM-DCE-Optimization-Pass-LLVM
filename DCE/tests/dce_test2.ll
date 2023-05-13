; ModuleID = './tests/dce_test2.bc'
source_filename = "./tests/dce_test2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@main.loop_exit = internal global i32 0, align 4

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %type = alloca i8, align 1
  %op2 = alloca double, align 8
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %f = alloca i32, align 4
  %g = alloca i32, align 4
  %h = alloca i32, align 4
  %i = alloca i8, align 1
  %j = alloca i8, align 1
  %k = alloca i8, align 1
  %s = alloca [200 x i8], align 16
  store i32 0, ptr %retval, align 4
  store i8 48, ptr %type, align 1
  store i8 1, ptr %k, align 1
  store i32 0, ptr %d, align 4
  store i32 1, ptr %b, align 4
  %0 = load i32, ptr %b, align 4
  %1 = load i32, ptr %d, align 4
  %sub = sub nsw i32 %0, %1
  store i32 %sub, ptr %c, align 4
  br label %do.body

do.body:                                          ; preds = %land.end, %entry
  %2 = load i8, ptr %type, align 1
  %conv = sext i8 %2 to i32
  switch i32 %conv, label %sw.default [
    i32 48, label %sw.bb
    i32 43, label %sw.bb7
    i32 42, label %sw.bb8
    i32 45, label %sw.bb10
    i32 47, label %sw.bb11
    i32 10, label %sw.bb12
  ]

sw.bb:                                            ; preds = %do.body
  store i32 1, ptr %a, align 4
  store i32 2, ptr %b, align 4
  store i32 25, ptr %c, align 4
  %3 = load i8, ptr %k, align 1
  %tobool = trunc i8 %3 to i1
  br i1 %tobool, label %if.then, label %if.else

if.then:                                          ; preds = %sw.bb
  %4 = load i32, ptr %a, align 4
  %5 = load i32, ptr %b, align 4
  %add = add nsw i32 %4, %5
  store i32 %add, ptr %c, align 4
  %6 = load i32, ptr %c, align 4
  %7 = load i32, ptr %a, align 4
  %sub1 = sub nsw i32 %6, %7
  store i32 %sub1, ptr %e, align 4
  br label %if.end6

if.else:                                          ; preds = %sw.bb
  store i32 8, ptr %d, align 4
  store i32 6, ptr %h, align 4
  store i32 24, ptr %f, align 4
  %8 = load i32, ptr %f, align 4
  %9 = load i32, ptr %h, align 4
  %add2 = add nsw i32 %8, %9
  store i32 %add2, ptr %e, align 4
  %10 = load i32, ptr %e, align 4
  %tobool3 = icmp ne i32 %10, 0
  br i1 %tobool3, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.else
  %11 = load i32, ptr %e, align 4
  %sub5 = sub nsw i32 %11, 1
  store i32 %sub5, ptr %e, align 4
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.else
  br label %if.end6

if.end6:                                          ; preds = %if.end, %if.then
  br label %sw.epilog

sw.bb7:                                           ; preds = %do.body
  br label %sw.epilog

sw.bb8:                                           ; preds = %do.body
  %12 = load i32, ptr %c, align 4
  %13 = load i32, ptr %d, align 4
  %sub9 = sub nsw i32 %12, %13
  store i32 %sub9, ptr %f, align 4
  br label %sw.epilog

sw.bb10:                                          ; preds = %do.body
  br label %sw.epilog

sw.bb11:                                          ; preds = %do.body
  br label %sw.epilog

sw.bb12:                                          ; preds = %do.body
  br label %sw.epilog

sw.default:                                       ; preds = %do.body
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.default, %sw.bb12, %sw.bb11, %sw.bb10, %sw.bb8, %sw.bb7, %if.end6
  %14 = load i32, ptr @main.loop_exit, align 4
  %inc = add nsw i32 %14, 1
  store i32 %inc, ptr @main.loop_exit, align 4
  br label %do.cond

do.cond:                                          ; preds = %sw.epilog
  %15 = load i32, ptr %a, align 4
  %tobool13 = icmp ne i32 %15, 0
  br i1 %tobool13, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %do.cond
  %16 = load i32, ptr @main.loop_exit, align 4
  %cmp = icmp slt i32 %16, 100
  br label %land.end

land.end:                                         ; preds = %land.rhs, %do.cond
  %17 = phi i1 [ false, %do.cond ], [ %cmp, %land.rhs ]
  br i1 %17, label %do.body, label %do.end, !llvm.loop !6

do.end:                                           ; preds = %land.end
  %18 = load i32, ptr %d, align 4
  %19 = load i32, ptr %f, align 4
  %add15 = add nsw i32 %18, %19
  store i32 %add15, ptr %e, align 4
  ret i32 0
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
