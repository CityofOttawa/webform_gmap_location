<?php
define('GMAP_DEFAULT_LAT', 48.00);
define('GMAP_DEFAULT_LON', 37.80);
define('GMAP_DEFAULT_ZOOM', 10);


/**
 * Specify the default properties of a component.
 * @return
 *   An array defining the default structure of a component.
 */
function _webform_defaults_gmap() {
  return array(
    'name'      => '',
    'form_key'  => NULL,
    'required'  => 0,
    'mandatory' => 0,
    'pid'       => 0,
    'weight'    => 0,
    'extra'     => array(
      'title_display'              => 0,
      'private'                    => FALSE,
      'attributes'                 => array(),
      'width'                      => 600,
      'height'                     => 300,
      'marker'                     => 0,
      'gmap'                       => '',
      'location'                   => '',
      'gmap_lat'                   => '',
      'gmap_lon'                   => '',
      'gmap_zoom'                  => '',
      'places_autocomplete'        => 0,

    ),
  );
}


/**
 * Implements _webform_theme_component().
 */
function _webform_theme_gmap() {
  return array(
    'gmap_location' => array(
      'render element' => 'element',
    ),
  );
}


/**
 * Generate the form for editing a component.
 * @param $component
 *   A Webform component array.
 *
 * @return
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_gmap($component) {
  global $language;

  $path = drupal_get_path('module', 'webform_gmap_location');
  drupal_add_js('http://maps.google.com/maps/api/js?v=3&sensor=false&libraries=places&language='.$language->language,  array('type' => 'external', 'weight' => 5));
  drupal_add_js($path . '/js/gmap_admin.js', array('type' => 'file', 'weight' => 6, 'scope' => 'footer'));


  drupal_add_js(
    array('gmap' =>
    array(
      'lat' => empty($component['extra']['gmap_lat']) ? GMAP_DEFAULT_LAT : $component['extra']['gmap_lat'],
      'lon'=> empty($component['extra']['gmap_lon']) ? GMAP_DEFAULT_LON : $component['extra']['gmap_lon'],
      'path' => get_marker_url('webform_gmap_location_marker_fid'),
      'zoom'=> empty($component['extra']['gmap_zoom']) ? GMAP_DEFAULT_ZOOM : $component['extra']['gmap_zoom'],
    )
    ),
    'setting');

  $form = array();
  $form['extra']['places_autocomplete'] = array(
    '#type' =>'checkbox',
    '#default_value' => $component['extra']['places_autocomplete'],
    '#title' => t('Enable places autocomplete.'),
  );
  $form['extra']['width'] = array(
    '#type'              => 'textfield',
    '#title'             => t('Default width'),
    '#default_value'     => $component['extra']['width'],
    '#description'       => t('Enter the width map in px. If you need 100% width - set to 0.'),
    '#size'              => 10,
    '#field_suffix'      => 'px',
  );
  $form['extra']['height'] = array(
    '#type'              => 'textfield',
    '#title'             => t('Default height'),
    '#default_value'     => $component['extra']['height'],
    '#description'       => t('Enter the height map in px.'),
    '#size'              => 10,
    '#field_suffix'      => 'px',
  );
  $form['extra']['marker'] = array(
    '#title'             => t('Marker'),
    '#type'              => 'managed_file',
    '#description'       => t('Download the map marker.'),
    '#default_value'     => variable_get('webform_gmap_location_marker_fid', ''),
    '#upload_location'   => 'public://webform_gmap_location/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('jpg png jpeg gif'),
    ),
  );
  $form['extra']['location'] = array(
    '#type'              => 'textfield',
    '#title'             => t('Default location'),
    '#default_value'     => $component['extra']['location'],
    '#description'       => t('Enter default location of the marker on the map.'),
    '#attributes' => array(
      'class' => array(
        'field-type-places-autocomplete',
      ),
      'id' => 'MapLocation'
    ),
  );
  $form['extra']['gmap'] = array(
    '#markup' => '<div id="gmap" style="width:100%;height:400px;"></div>',
  );
  $form['extra']['gmap_lat'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($component['extra']['gmap_lat']) ? GMAP_DEFAULT_LAT : $component['extra']['gmap_lat'],
    '#attributes' => array('id'=>'gmap_lat')
  );
  $form['extra']['gmap_lon'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($component['extra']['gmap_lon']) ? GMAP_DEFAULT_LON : $component['extra']['gmap_lon'],
    '#attributes' => array('id'=>'gmap_lon')
  );
  $form['extra']['gmap_zoom'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($component['extra']['gmap_zoom']) ? GMAP_DEFAULT_ZOOM : $component['extra']['gmap_zoom'],
    '#attributes' => array('id'=>'gmap_zoom')
  );


  return $form;
}


/**
 * Render a Webform component to be part of a form.
 * @see _webform_client_form_add_component()
 */
function _webform_render_gmap($component, $value = NULL, $filter = TRUE) {
  global $language;

  $path = drupal_get_path('module', 'webform_gmap_location');
  drupal_add_js('http://maps.google.com/maps/api/js?v=3&sensor=false&libraries=places&language='.$language->language,  array('type' => 'external', 'weight' => 5));
  drupal_add_js($path . '/js/gmap_admin.js', array('type' => 'file', 'weight' => 6, 'scope' => 'footer'));

  drupal_add_js(
    array('gmap' =>
    array(
      'lat' => empty($component['extra']['gmap_lat']) ? GMAP_DEFAULT_LAT : $component['extra']['gmap_lat'],
      'lon'=> empty($component['extra']['gmap_lon']) ? GMAP_DEFAULT_LON : $component['extra']['gmap_lon'],
      'path' => get_marker_url('webform_gmap_location_marker_fid'),
      'zoom'=> empty($component['extra']['gmap_zoom']) ? GMAP_DEFAULT_ZOOM : $component['extra']['gmap_zoom'],
    )
    ),
    'setting');

  $form_item = array(
    '#type'             => 'markup',
    '#default_value'    => $filter ? _webform_filter_values($component['value']) : $component['value'],
    '#attributes'       => $component['extra']['attributes'],
    '#theme_wrappers'   => array( 'webform_element' ),
    '#required'         => $component['required'] || $component['mandatory'],//Either one being true will could as required...because webform changed in 4.x-alpha8
    '#title'            => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display'    => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight'           => $component['weight'],
    '#markup'           =>  get_markup($component),
    '#translatable'     => array('title',),
    '#webform_component' => $component,
    '#theme' => 'gmap_location',
    '#process'          => array('webform_expand_hidden_field'),
  );

  if ( isset( $value ) ) {
    $form_item['#default_value'] = $value[0];
  }

  return $form_item;
}


/**
 * @param $element
 * @return mixed
 * Add 3 hidden fields and loacation field
 */
function webform_expand_hidden_field($element) {
  $component = $element["#webform_component"];
  if($component["extra"]["places_autocomplete"] == 1) {
    $element['location'] = array(
      '#type'              => 'textfield',
      '#title'             => t('Location'),
      '#description'       => t('Enter location on the map.'),
      '#attributes' => array(
        'class' => array(
          'field-type-places-autocomplete',
        ),
        'id' => 'MapLocation'
      ),
    );
  }
  $element['gmap_lat'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($component['extra']['gmap_lat']) ? GMAP_DEFAULT_LAT : $component['extra']['gmap_lat'],
    '#attributes' => array('id'=>'gmap_lat')
  );
  $element['gmap_lon'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($component['extra']['gmap_lon']) ? GMAP_DEFAULT_LON : $component['extra']['gmap_lon'],
    '#attributes' => array('id'=>'gmap_lon')
  );
  $element['gmap_zoom'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($component['extra']['gmap_zoom']) ? GMAP_DEFAULT_ZOOM : $component['extra']['gmap_zoom'],
    '#attributes' => array('id'=>'gmap_zoom'),
  );
  return $element;
}


/**
 * Format the output of data for this component.
 */
function theme_gmap_location($variables) {
  $element = $variables['element'];
  return '<div class="webform-container-inline">' . $element["#markup"] . drupal_render_children($element) . '</div>';
}



/**
 * Calculate and returns statistics about results for this component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @param $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will
 *   be limited to these sids.
 * @param $single
 *   Boolean flag determining if the details about a single component are being
 *   shown. May be used to provided detailed information about a single
 *   component's analysis, such as showing "Other" options within a select list.
 *
 * @return
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_gmap($component, $sids = array(), $single = FALSE) {
  // Generate the list of options and questions.
  $query = db_select('webform_submitted_data', 'wsd', array( 'fetch' => PDO::FETCH_ASSOC ))->fields('wsd', array( 'data' ))->condition('nid', $component['nid'])->condition('cid', $component['cid']);
  if ( count($sids) ) {
    $query->condition('sid', $sids, 'IN');
  }
  $non_blanks = 0;
  $submissions = 0;
  $result = $query->execute();
  foreach ( $result as $data ) {
    if ( drupal_strlen(trim($data['data'])) > 0 ) {
      $non_blanks++;
    }
    $submissions++;
  }
  $rows[0] = array(
    t('Left Blank'),
    ( $submissions - $non_blanks )
  );
  $rows[1] = array(
    t('User entered value'),
    $non_blanks
  );
  return $rows;
}


/**
 * Return the result of a component value for display in a table.
 * The output of this function will be displayed under the "Results" tab then
 * "Table".
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 *
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_table_gmap($component, $value) {
  return check_plain(empty( $value[0] ) ? '' : $value[0]);
}

/**
 * Return the header for this component to be displayed in a CSV file.
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param $component
 *   A Webform component array.
 * @param $export_options
 *   An array of options that may configure export of this field.
 *
 * @return
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_gmap($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}


/**
 * Format the submitted data of a component for CSV downloading.
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param $component
 *   A Webform component array.
 * @param $export_options
 *   An array of options that may configure export of this field.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 *
 * @return
 *   An array of items to be added to the CSV file. Each value within the array
 *   will be another column within the file. This function is called once for
 *   every row of data.
 */
function _webform_csv_data_gmap($component, $export_options, $value) {
  return !isset( $value[0] ) ? '' : $value[0];
}



/**
 * @param $component
 * @return string
 * return markup value
 */
function get_markup($component) {
  $width = (int)$component['extra']['width']!=0 ? (int)$component['extra']['width'] : '100%';
  $width = $width ? $width.'px': '100%';
  $height = (int)$component['extra']['height'] ? (int)$component['extra']['height'].'px': '300px';

  return '<div id="gmap" style="display: block; width:'.$width.';height:'.$height.';"></div>';
}

/**
 * @param $name_field
 * @return bool|string
 * Return marker url
 */
function get_marker_url($name_field) {
  if(variable_get($name_field) != '') {
    $fid = variable_get('webform_gmap_location_marker_fid', '');
    $file = file_load($fid);
    return file_create_url($file->uri);
  }
  $path = drupal_get_path('module', 'webform_gmap_location');
  return file_create_url($path.'/img/marker.png');
}
