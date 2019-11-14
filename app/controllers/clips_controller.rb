class ClipsController < ApplicationController
  before_action :authenticate_user!
  before_action :is_admin
  before_action :set_clip, only: [:edit, :update, :destroy]

  # GET /clips
  # GET /clips.json
  def index
    @clips = Clip.all.order("id")
  end

  # GET /clips/new
  def new
    @clip = Clip.find_or_create_by(name: "new clip")
    redirect_to clips_path
  end

  # GET /clips/1/edit
  def edit
  end

  # PATCH/PUT /clips/1
  # PATCH/PUT /clips/1.json
  def update
    respond_to do |format|
      if @clip.update(clip_params)
        format.html { redirect_to clips_path, notice: 'Clip was successfully updated.' }
        format.json { render :show, status: :ok, location: @clip }
      else
        format.html { render :edit }
        format.json { render json: @clip.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /clips/1
  # DELETE /clips/1.json
  def destroy
    @clip.destroy
    respond_to do |format|
      format.html { redirect_to clips_path, notice: 'Clip was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_clip
      @clip = Clip.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def clip_params
      params.require(:clip).permit(:name, :path, :gain, :start, :end, :bpm)
    end

    def is_admin
      if current_user != User.find_by(name: "Mitch")
        redirect_to channels_path
      end
  end
end
