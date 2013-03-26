<?php
/**
 * Models a projection, a description of a coordinate system.
 */
class openlayers_projection {
  /**
   * @var String Opaque primary key (should not be exposed but ctools API doesn't allow for a more rigid structure)
   */
  public $identifier;

  /**
   * @var String proj4 definition for on-the-fly reprojections of vector data.
   */
  private $definition;

  /**
   * @var number Leftmost boundary where coordinate system is valid
   */
  private $projectedextentleft;
  /**
   * @var number Bottommost boundary where coordinate system is valid
   */
  private $projectedextentbottom;
  /**
   * @var number Rightmost boundary where coordinate system is valid
   */
  private $projectedextentright;
  /**
   * @var number Topmost boundary where coordinate system is valid
   */
  private $projectedextenttop;

  public function __construct($identifier, $definition, $projectedextentleft, $projectedextentbottom, $projectedextentright, $projectedextenttop) {
    $this->identifier = $identifier;
    $this->definition = $definition;
    $this->projectedextentleft = $projectedextentleft;
    $this->projectedextentbottom = $projectedextentbottom;
    $this->projectedextentright = $projectedextentright;
    $this->projectedextenttop = $projectedextenttop;
  }

  /**
   * @return string Textual representation for the user.
   */
  public function getLocalizedMessage() {
    return $this->identifier;
  }

  /**
   * @return array Boundaries of projection in projected coordinates
   */
  public function getProjectedExtent() {
    return array_map('floatval', array($this->projectedextentleft, $this->projectedextentbottom, $this->projectedextentright, $this->projectedextenttop));
  }

  /**
   * @return String Proj4 style definition
   */
  public function getDefinition() {
    return $this->definition;
  }
}
