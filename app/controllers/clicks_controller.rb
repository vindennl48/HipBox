class ClicksController < ApplicationController
  before_action :authenticate_user!
  before_action :is_admin
  before_action :set_click, only: [:edit, :update, :destroy]

  # GET /clicks
  # GET /clicks.json
  def index
    @clicks = Click.all
  end

  # GET /clicks/new
  def new
    @click = Click.find_or_create_by(name: "new click")
    redirect_to samples_path
  end

  # GET /clicks/1/edit
  def edit
  end

  # PATCH/PUT /clicks/1
  # PATCH/PUT /clicks/1.json
  def update
    respond_to do |format|
      if @click.update(click_params)
        format.html { redirect_to samples_path, notice: 'Click was successfully updated.' }
        format.json { render :show, status: :ok, location: @click }
      else
        format.html { render :edit }
        format.json { render json: @click.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /clicks/1
  # DELETE /clicks/1.json
  def destroy
    @click.destroy
    respond_to do |format|
      format.html { redirect_to samples_path, notice: 'Click was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_click
      @click = Click.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def click_params
      params.require(:click).permit(:name, :path, :gain, :is_high, :is_active)
    end

    def is_admin
      if current_user != User.find_by(name: "Mitch")
        redirect_to channels_path
      end
  end
end
