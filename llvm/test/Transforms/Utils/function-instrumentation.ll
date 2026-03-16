; RUN: opt -S -passes=function-instrumentation %s | FileCheck %s

define i32 @foo(i32 %x) {
entry:
  %add = add i32 %x, 1
  ret i32 %add
}

define i32 @bar(i1 %cond) {
entry:
  br i1 %cond, label %yes, label %no

yes:
  ret i32 1

no:
  ret i32 0
}

define void @instrument_start() {
entry:
  ret void
}

define void @instrument_end() {
entry:
  ret void
}

; CHECK-LABEL: define i32 @foo(i32 %x) {
; CHECK: entry:
; CHECK-NEXT: call void @instrument_start()
; CHECK-NEXT: %add = add i32 %x, 1
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret i32 %add

; CHECK-LABEL: define i32 @bar(i1 %cond) {
; CHECK: entry:
; CHECK-NEXT: call void @instrument_start()
; CHECK-NEXT: br i1 %cond, label %yes, label %no
; CHECK: yes:
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret i32 1
; CHECK: no:
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret i32 0

; CHECK-LABEL: define void @instrument_start() {
; CHECK: entry:
; CHECK-NEXT: ret void

; CHECK-LABEL: define void @instrument_end() {
; CHECK: entry:
; CHECK-NEXT: ret void
