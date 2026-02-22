// RUN: %clang -fplugin=%llvmshlibdir/TypeInfoPrinter%pluginext -fsyntax-only %s | FileCheck %s
// REQUIRES: plugins, examples

struct Human {
  unsigned age;
  unsigned height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};

struct Engineer : Human {
  unsigned salary;
  void sleep() override {}
  void eat() override {}
  void work() {}
};

struct SkibidiAcademy {
  unsigned ohio_points;
protected:
  unsigned sigma_aura;
private:
  [[maybe_unused]] unsigned secret_gyatt;
public:
  virtual int level_of_rizz(int x) { return x; }
  void no_cap(unsigned x, const char *name) {}
};

struct GigachadEngineer : Engineer, SkibidiAcademy {
  void sleep() override {}
  void eat() override {}
  int level_of_rizz(int x) override { return x; }
};

struct SusMethods {
  SusMethods() {}
  ~SusMethods() {}
  operator int() const { return 42; }
  void touch_grass() {}
};

// CHECK-LABEL: Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ age (unsigned int|public)
// CHECK-NEXT: | |_ height (unsigned int|public)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ sleep (void()|public|virtual|pure)
// CHECK-NEXT: | |_ eat (void()|public|virtual|pure)
//
// CHECK-LABEL: Engineer -> Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ salary (unsigned int|public)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ sleep (void()|public|override)
// CHECK-NEXT: | |_ eat (void()|public|override)
// CHECK-NEXT: | |_ work (void()|public)
//
// CHECK-LABEL: SkibidiAcademy
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ ohio_points (unsigned int|public)
// CHECK-NEXT: | |_ sigma_aura (unsigned int|protected)
// CHECK-NEXT: | |_ secret_gyatt (unsigned int|private)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ level_of_rizz (int(int)|public|virtual)
// CHECK-NEXT: | |_ no_cap (void(unsigned int, const char *)|public)
//
// CHECK-LABEL: GigachadEngineer -> Engineer, SkibidiAcademy
// CHECK-NEXT: |_Fields
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ sleep (void()|public|override)
// CHECK-NEXT: | |_ eat (void()|public|override)
// CHECK-NEXT: | |_ level_of_rizz (int(int)|public|override)
//
// CHECK-LABEL: SusMethods
// CHECK-NEXT: |_Fields
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ touch_grass (void()|public)
// CHECK-NOT: | |_ SusMethods (
// CHECK-NOT: | |_ ~SusMethods (
// CHECK-NOT: | |_ operator
