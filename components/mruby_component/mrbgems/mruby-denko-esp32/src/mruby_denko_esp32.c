#include <mruby.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/variable.h>
#include <mruby/value.h>

#include <esp_rom_sys.h>
// From picoruby/mrbgems
#include <gpio.h>
#include <adc.h>
#include <pwm.h>

static mrb_value
denko_board_digital_write(mrb_state* mrb, mrb_value self) {
  mrb_int pin, val;
  mrb_get_args(mrb, "ii", &pin, &val);

  // From picoruby-gpio esp32 port
  GPIO_write(pin, val);
  return mrb_nil_value();
}

static mrb_value
denko_board_digital_read_raw(mrb_state* mrb, mrb_value self) {
  mrb_int pin;
  mrb_get_args(mrb, "i", &pin);
  // From picoruby-gpio
  return mrb_fixnum_value(GPIO_read(pin));
}

static mrb_value
denko_board_analog_read_raw(mrb_state* mrb, mrb_value self) {
  mrb_int pin;
  mrb_get_args(mrb, "i", &pin);
  // From picoruby-adc
  return mrb_fixnum_value(ADC_read_raw(pin));
}

static mrb_value
denko_board_micro_delay(mrb_state *mrb, mrb_value self) {
  mrb_int microseconds;
  mrb_get_args(mrb, "i", &microseconds);
  esp_rom_delay_us(microseconds);
  return self;
}

static mrb_value
denko_board_pwm_enable(mrb_state *mrb, mrb_value self) {
  mrb_int pin;
  mrb_bool state;
  mrb_get_args(mrb, "ib", &pin, &state);
  PWM_set_enabled(pin, state);
  return self;
}
void
mrb_mruby_denko_esp32_gem_init(mrb_state* mrb) {
  // Denko module
  struct RClass *mrb_Denko = mrb_define_module(mrb, "Denko");

  // Denko::Board class
  struct RClass *mrb_Denko_Board = mrb_define_class_under(mrb, mrb_Denko, "Board", mrb->object_class);

  // System
  mrb_define_method(mrb, mrb_Denko_Board, "micro_delay",    denko_board_micro_delay,    MRB_ARGS_REQ(1));

  // DigitalIO
  mrb_define_method(mrb, mrb_Denko_Board, "digital_write",    denko_board_digital_write,    MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_Denko_Board, "digital_read_raw", denko_board_digital_read_raw, MRB_ARGS_REQ(1));

  // AnalogIO
  mrb_define_method(mrb, mrb_Denko_Board, "analog_read_raw",  denko_board_analog_read_raw,  MRB_ARGS_REQ(1));

  // PulseIO
  mrb_define_method(mrb, mrb_Denko_Board, "pwm_enable",       denko_board_pwm_enable,       MRB_ARGS_REQ(2));
}

void
mrb_mruby_denko_esp32_gem_final(mrb_state* mrb) {
}
