require "sdl2"

SDL2.init(SDL2::INIT_EVERYTHING)

window = SDL2::Window.create("Sin Wave",
  SDL2::Window::POS_CENTERED, SDL2::Window::POS_CENTERED,
  150, 100, 0)
renderer = window.create_renderer(-1, 0)

class SinWave
  attr_accessor :x, :y, :div
  attr_reader :ix, :iy
  def initialize(x, y, color, divisor=2.5)
    @ix, @iy = x, y
    @x, @y, @color = x, y, color
    @div = divisor
  end

  def dist(nx, ny)
    Math.sqrt((@x-nx)**2 + (@y-ny)**2)
  end

  def color_at(x, y)
    r, g, b = @color
    cr = (Math.sin(dist(x, y)/@div) + 1) * r/2
    cg = (Math.sin(dist(x, y)/@div) + 1) * g/2
    cb = (Math.sin(dist(x, y)/@div) + 1) * b/2
    [cr, cg, cb]
  end

  def newpos!(i, range = 30)
    @x = @ix + Math.sin(i) * range
    @y = @iy + Math.sin(i) * range
  end
end

class MixWave
  def initialize(renderer)
    @r = renderer

    @rw = SinWave.new(-20, 0, [255, 0, 0], 6.5)
    @gw = SinWave.new(15, 2, [0, 255, 0], 5.0)
    @bw = SinWave.new(8, 15, [0, 0, 255], 4.8)

    @ri = 0.0
    @gi = 0.0
    @bi = 0.0
  end

  def set(x, y, c)
    @r.draw_color = (c.size == 4 ? c : c + [255])
    @r.fill_rect(SDL2::Rect.new(x*10, y*10, 10, 10))
  end

  def draw
    @r.draw_color = [0, 0, 0, 255]
    @r.clear

    for x in 0...15
      for y in 0...10
        red   = @rw.color_at(x, y)
        green = @gw.color_at(x, y)
        blue  = @bw.color_at(x, y)
        c = [
          (red[0] + green[0] + blue[0]) / 3,
          (red[1] + green[1] + blue[1]) / 3,
          (red[2] + green[2] + blue[2]) / 3
        ]
        set(x, y, c)
      end
    end

    @rw.newpos!(@ri, 30)
    @gw.newpos!(@gi, 20)
    @bw.newpos!(@bi, 25)

    @ri += 0.05
    @gi += 0.03
    @bi += 0.06
  end
end

mix = MixWave.new(renderer)

loop do
  while ev = SDL2::Event.poll
    if SDL2::Event::KeyDown === ev
      exit
    end
  end

  mix.draw

  renderer.present

  sleep 0.1
end