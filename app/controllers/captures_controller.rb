class CapturesController < ApplicationController
  protect_from_forgery :except => [:create]
  def index
    @captures = Capture.all
    render json: @captures
  end

  def show
    @capture = Capture.find(params[:id])
  end

  def new
    @capture = Capture.new
  end

  def create
    @capture = Capture.new(capture_params)
    if @capture.save
      render json: @capture
    else
      render 'new'
    end
  end

  private
    def capture_params
      params.require(:capture).permit(:capture)
    end
end
